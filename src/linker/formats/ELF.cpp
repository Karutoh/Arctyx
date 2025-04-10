#include "arctyx/linker/formats/ELF.h"

ELF::ELF()
	: bitDepth(ELF_BIT_DEPTH_64), endianness(ELF_END_LE), abi(ELFH_ABI_SYSTEMV), type(ELFH_TYPE_EXEC),
	arch(ELF_ARCH_X86_64), entryPoint(0)
{
	InitializeSections();
}

ELF::ELF(const ehs::UInt_8 &bitDepth, const ehs::UInt_8 &end, const ehs::UInt_8 &abi, const ehs::UInt_16 &type, const ehs::UInt_16 &arch)
	: bitDepth(bitDepth), endianness(end), abi(abi), type(type), arch(arch), entryPoint(0)
{
	InitializeSections();
}

ELF::ELF(ehs::Serializer<ehs::UInt_64>& data)
{
}

ELF::ELF(ELF&& header) noexcept
	: bitDepth(header.bitDepth), endianness(header.endianness), abi(header.abi), type(header.type), arch(header.arch),
	entryPoint(header.entryPoint), programs((ehs::Vector<ELF_Program, ehs::UInt_16>&&)header.programs),
	sections((ehs::Vector<ELF_Section, ehs::UInt_16>&&)header.sections)
{
	header.bitDepth = ELF_BIT_DEPTH_64;
	header.endianness = ELF_END_LE;
	header.abi = ELFH_ABI_SYSTEMV;
	header.type = ELFH_TYPE_EXEC;
	header.arch = ELF_ARCH_X86_64;
	header.entryPoint = 0;

	header.InitializeSections();
}

ELF::ELF(const ELF& header)
	: bitDepth(header.bitDepth), endianness(header.endianness), abi(header.abi), type(header.type), arch(header.arch),
	entryPoint(header.entryPoint), programs(header.programs), sections(header.sections)
{
}

ELF& ELF::operator=(ELF&& header) noexcept
{
	if (this == &header)
		return *this;

	bitDepth = header.bitDepth;
	endianness = header.endianness;
	abi = header.abi;
	type = header.type;
	arch = header.arch;
	entryPoint = header.entryPoint;
	programs = (ehs::Vector<ELF_Program, ehs::UInt_16>&&)header.programs;
	sections = (ehs::Vector<ELF_Section, ehs::UInt_16>&&)header.sections;

	header.bitDepth = ELF_BIT_DEPTH_64;
	header.endianness = ELF_END_LE;
	header.abi = ELFH_ABI_SYSTEMV;
	header.type = ELFH_TYPE_EXEC;
	header.arch = ELF_ARCH_X86_64;
	header.entryPoint = 0;

	header.InitializeSections();

	return *this;
}

ELF& ELF::operator=(const ELF& header)
{
	if (this == &header)
		return *this;

	bitDepth = header.bitDepth;
	endianness = header.endianness;
	abi = header.abi;
	type = header.type;
	arch = header.arch;
	entryPoint = header.entryPoint;
	programs = header.programs;
	sections = header.sections;

	return *this;
}

ehs::UInt_8 ELF::GetBitDepth() const
{
	return bitDepth;
}

ehs::UInt_8 ELF::GetEndianness() const
{
	return endianness;
}

ehs::UInt_8 ELF::GetABI() const
{
	return abi;
}

ehs::UInt_16 ELF::GetType() const
{
	return type;
}

ehs::UInt_16 ELF::GetArchitecture() const
{
	return arch;
}

void ELF::SetEntryPoint(ehs::UInt_16 programIndex)
{
	if (programIndex > programs.End())
	{
		EHS_LOG(ehs::LogType::WARN, 0, "The given index, \"" + ehs::Str_8::FromNum(programIndex) +
							  "\", is outside the bounds of, \"" + ehs::Str_8::FromNum(programs.End()) + "\".");

		return;
	}

	entryPoint = programIndex;

	EHS_LOG_SUCCESS();
}

ELF_Program* ELF::GetEntryPoint() const
{
	return &programs[entryPoint];
}

void ELF::AddProgram(ELF_Program newProgram)
{
	/*
	for (ehs::UInt_16 i = 0; i < programs.Size(); i++)
	{
		if (programs[i].fileSize < newProgram.fileSize)
			continue;

		EHS_LOG("Warning", 0, "The new program file size, \"" + ehs::Str_8::FromNum(newProgram.fileSize) +
							  "\" must be greater than, \"" + ehs::Str_8::FromNum(programs[i].fileSize) + "\".");

		return;
	}
	*/

	programs.Push((ELF_Program&&)newProgram);
}

bool ELF::HasSection(ehs::UInt_64 hashId)
{
	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
		if (sections[i].GetHashId() == hashId)
			return true;
	
	return false;
}

bool ELF::HasSection(ehs::Str_8& id)
{
	return HasSection(id.Hash_64());
}

void ELF::AddSection(ELF_Section newSection)
{
	if (HasSection(newSection.GetHashId()))
	{
		EHS_LOG(ehs::LogType::WARN, 0, "The section, \"" + newSection.GetId() + "\", already exists.");
		return;
	}

	ehs::Str_8 newId = newSection.GetId();

	ELF_Section* shStrTab = FindShStrTab();
	ehs::Serializer<ehs::UInt_64>& data = shStrTab->GetData();

	newSection.nameOffset = data.GetOffset();

	data.Resize(data.Size() + newId.Size() + 1);
	ehs::Util::Copy(&data[data.GetOffset()], &newId[0], newId.Size());
	data.SetOffset(data.GetOffset() + newId.Size());
	data.Write('\0');
	
	sections.Push((ELF_Section&&)newSection);

	EHS_LOG_SUCCESS();
}

ELF_Section* ELF::GetSection(ehs::UInt_64 sectionHashId) const
{
	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
		if (sections[i].GetHashId() == sectionHashId)
			return &sections[i];

	return nullptr;
}

ELF_Section* ELF::GetSection(const ehs::Str_8& sectionId) const
{
	return GetSection(sectionId.Hash_64());
}

ehs::UInt_64 ELF::GetLastSegmentOffset() const
{
	ehs::UInt_64 offset = ELF_64_BIT_HEADER_SIZE + PRGH_SIZE * programs.Size();

	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
	{
		if (sections[i].fileOffset)
			offset = sections[i].fileOffset;

		offset += sections[i].GetData().Size();
	}

	return offset;
}

ehs::Vec2_u64 ELF::SegmentRange(ehs::UInt_64 sectionHashId)
{
	ehs::UInt_64 offset = ELF_64_BIT_HEADER_SIZE + PRGH_SIZE * programs.Size();
	if (sectionHashId)
	{
		for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
		{
			if (sections[i].GetHashId() == sectionHashId)
				return {offset, sections[i].GetData().Size()};
			else
			{
				if (sections[i].fileOffset)
					offset = sections[i].fileOffset;

				offset += sections[i].GetData().Size();
			}
		}
	}
	else
	{
		ehs::UInt_64 scale = 0;

		for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
			scale += sections[i].GetData().Size();

		return {offset, scale};
	}

	return {};
}

ehs::Vec2_u64 ELF::SegmentRange(const ehs::Str_8& sectionId)
{
	return SegmentRange(sectionId.Hash_64());
}

ehs::Serializer<ehs::UInt_64> ELF::Serialize() const
{
	ehs::Serializer<ehs::UInt_64> result(ehs::Endianness::LE);

	if (bitDepth == ELF_BIT_DEPTH_64)
		Serialize_64(result);
	else if (bitDepth == ELF_BIT_DEPTH_32)
		Serialize_32(result);

	for (ehs::UInt_16 i = 0; i < programs.Size(); i++)
		programs[i].Serialize(bitDepth, result);

	for (ehs::UInt_16 i = 1; i < sections.Size(); i++)
	{
		if (sections[i].fileOffset)
		{
			ehs::UInt_64 padding = sections[i].fileOffset - result.Size();
			result.Resize(result.Size() + padding);
			ehs::Util::Zero(&result[result.GetOffset()], padding);
			result.SetOffset(result.GetOffset() + padding);
		}

		sections[i].segmentOffset = result.GetOffset();
		result.WriteSer(sections[i].GetData());
	}

	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
		sections[i].Serialize(bitDepth, result);
	
	return result;
}

void ELF::Serialize_64(ehs::Serializer<ehs::UInt_64> &data) const
{
	data.Resize(ELF_64_BIT_HEADER_SIZE);

	// ELF magic number.
	data.Write<ehs::UInt_32>(0x464C457F);

	data.Write<ehs::UInt_8>(bitDepth);
	data.Write(endianness);
	data.Write<ehs::UInt_8>(1);
	data.Write(abi);
	data.Write<ehs::UInt_8>(0);

	ehs::Util::Zero(&data[data.GetOffset()], 7);
	data.SetOffset(data.GetOffset() + 7);

	data.Write(type);
	data.Write(arch);
	data.Write<ehs::UInt_32>(1);
	data.Write(programs[entryPoint].pAddr);
	data.Write<ehs::UInt_64>(ELF_64_BIT_HEADER_SIZE);
	data.Write(RetrieveSectionsOffset());
	data.Write<ehs::UInt_32>(0);
	data.Write<ehs::UInt_16>(ELF_64_BIT_HEADER_SIZE);
	data.Write<ehs::UInt_16>(PRGH_SIZE);
	data.Write(programs.Size());
	data.Write<ehs::UInt_16>(SECH_SIZE);
	data.Write(sections.Size());
	data.Write(FindShStrTabIndex());
}

void ELF::Serialize_32(ehs::Serializer<ehs::UInt_64> &data) const
{
	data.Resize(ELF_32_BIT_HEADER_SIZE);

	// ELF magic number.
	data.Write<ehs::UInt_32>(0x464C457F);

	data.Write<ehs::UInt_8>(bitDepth);
	data.Write(endianness);
	data.Write<ehs::UInt_8>(1);
	data.Write(abi);
	data.Write<ehs::UInt_8>(0);

	ehs::Util::Zero(&data[data.GetOffset()], 7);
	data.SetOffset(data.GetOffset() + 7);

	data.Write(type);
	data.Write(arch);
	data.Write<ehs::UInt_32>(1);
	data.Write(programs[entryPoint].pAddr);

	if (bitDepth == ELF_BIT_DEPTH_64)
		data.Write<ehs::UInt_64>(ELF_64_BIT_HEADER_SIZE);
	else if (bitDepth == ELF_BIT_DEPTH_32)
		data.Write<ehs::UInt_32>(ELF_32_BIT_HEADER_SIZE);

	data.Write(RetrieveSectionsOffset());
	data.Write<ehs::UInt_32>(0);

	if (bitDepth == ELF_BIT_DEPTH_64)
		data.Write<ehs::UInt_16>(ELF_64_BIT_HEADER_SIZE);
	else if (bitDepth == ELF_BIT_DEPTH_32)
		data.Write<ehs::UInt_16>(ELF_32_BIT_HEADER_SIZE);

	data.Write<ehs::UInt_16>(PRGH_SIZE);
	data.Write(programs.Size());
	data.Write<ehs::UInt_16>(SECH_SIZE);
	data.Write(sections.Size());
	data.Write(FindShStrTabIndex());
}

void ELF::InitializeSections()
{
	ehs::Str_8 nullName = ".NULL";

	ELF_Section null(nullName, 0, SECH_TYPE_NULL, 0, 0, 0);

	ehs::Str_8 shStrTabName = ".shstrtab";

	ELF_Section shStrTab(shStrTabName, 0, SECH_TYPE_STRTAB, SECH_FLAG_STRINGS, 0, 1);

	ehs::Serializer<ehs::UInt_64>& data = shStrTab.GetData();

	data.Resize(nullName.Size() + shStrTabName.Size() + 2);

	null.nameOffset = data.GetOffset();
	ehs::Util::Copy(&data[data.GetOffset()], &nullName[0], nullName.Size());
	data.SetOffset(data.GetOffset() + nullName.Size());
	data.Write('\0');

	shStrTab.nameOffset = data.GetOffset();
	ehs::Util::Copy(&data[data.GetOffset()], &shStrTabName[0], shStrTabName.Size());
	data.SetOffset(data.GetOffset() + shStrTabName.Size());
	data.Write('\0');

	sections.Push((ELF_Section&&)null);
	sections.Push((ELF_Section&&)shStrTab);
}

ehs::UInt_64 ELF::RetrieveSectionsOffset() const
{
	ehs::UInt_64 result = ELF_64_BIT_HEADER_SIZE + PRGH_SIZE * programs.Size();

	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
	{
		if (sections[i].fileOffset)
			result = sections[i].fileOffset;

		result += sections[i].GetData().Size();
	}

	return result;
}

ehs::UInt_16 ELF::FindShStrTabIndex() const
{
	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
	{
		if (sections[i].GetHashId() == 7974729948679187860ull)
		{
			EHS_LOG_SUCCESS();

			return i;
		}
	}

	EHS_LOG(ehs::LogType::ERR, 0, "Could not find the \".shstrtab\" index.");

	return 0;
}

ELF_Section* ELF::FindShStrTab() const
{
	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
	{
		if (sections[i].GetHashId() == 7974729948679187860ull)
		{
			EHS_LOG_SUCCESS();

			return &sections[i];
		}
	}

	EHS_LOG(ehs::LogType::ERR, 0, "Could not find the \".shstrtab\".");

	return nullptr;
}
