#include "ELF64.h"

constexpr ehs::Char_8 Elf_Magic[] = "\x7F" "ELF";
constexpr ehs::UInt_8 Elf_Magic_Size = sizeof(Elf_Magic) - 1;

ELF64::ELF64()
	: endianness(ELFH_END_LE), abi(ELFH_ABI_SYSTEMV), type(ELFH_TYPE_EXEC),
	arch(ELFH_MARCH_AMD_X86_64), entryPoint(0)
{
	InitializeSections();
}

ELF64::ELF64(ehs::UInt_8 end, ehs::UInt_8 abi, ehs::UInt_16 type, ehs::UInt_16 arch)
	: endianness(end), abi(abi), type(type), arch(arch), entryPoint(0)
{
	InitializeSections();
}

ELF64::ELF64(ehs::Serializer<ehs::UInt_64>& data)
{
}

ELF64::ELF64(ELF64&& header) noexcept
	: endianness(header.endianness), abi(header.abi), type(header.type), arch(header.arch),
	entryPoint(header.entryPoint), programs((ehs::Vector<ELF64_Program, ehs::UInt_16>&&)header.programs),
	sections((ehs::Vector<ELF64_Section, ehs::UInt_16>&&)header.sections)
{
	header.endianness = ELFH_END_LE;
	header.abi = ELFH_ABI_SYSTEMV;
	header.type = ELFH_TYPE_EXEC;
	header.arch = ELFH_MARCH_AMD_X86_64;
	header.entryPoint = 0;

	header.InitializeSections();
}

ELF64::ELF64(const ELF64& header)
	: endianness(header.endianness), abi(header.abi), type(header.type), arch(header.arch),
	entryPoint(header.entryPoint), programs(header.programs), sections(header.sections)
{
}

ELF64& ELF64::operator=(ELF64&& header) noexcept
{
	if (this == &header)
		return *this;

	endianness = header.endianness;
	abi = header.abi;
	type = header.type;
	arch = header.arch;
	entryPoint = header.entryPoint;
	programs = (ehs::Vector<ELF64_Program, ehs::UInt_16>&&)header.programs;
	sections = (ehs::Vector<ELF64_Section, ehs::UInt_16>&&)header.sections;

	header.endianness = ELFH_END_LE;
	header.abi = ELFH_ABI_SYSTEMV;
	header.type = ELFH_TYPE_EXEC;
	header.arch = ELFH_MARCH_AMD_X86_64;
	header.entryPoint = 0;

	header.InitializeSections();

	return *this;
}

ELF64& ELF64::operator=(const ELF64& header)
{
	if (this == &header)
		return *this;

	endianness = header.endianness;
	abi = header.abi;
	type = header.type;
	arch = header.arch;
	entryPoint = header.entryPoint;
	programs = header.programs;
	sections = header.sections;

	return *this;
}

ehs::UInt_8 ELF64::GetEndianness() const
{
	return endianness;
}

ehs::UInt_8 ELF64::GetABI() const
{
	return abi;
}

ehs::UInt_16 ELF64::GetType() const
{
	return type;
}

ehs::UInt_16 ELF64::GetArchitecture() const
{
	return arch;
}

void ELF64::SetEntryPoint(ehs::UInt_16 programIndex)
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

ELF64_Program* ELF64::GetEntryPoint() const
{
	return &programs[entryPoint];
}

void ELF64::AddProgram(ELF64_Program newProgram)
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

	programs.Push((ELF64_Program&&)newProgram);
}

bool ELF64::HasSection(ehs::UInt_64 hashId)
{
	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
		if (sections[i].GetHashId() == hashId)
			return true;
	
	return false;
}

bool ELF64::HasSection(ehs::Str_8& id)
{
	return HasSection(id.Hash_64());
}

void ELF64::AddSection(ELF64_Section newSection)
{
	if (HasSection(newSection.GetHashId()))
	{
		EHS_LOG(ehs::LogType::WARN, 0, "The section, \"" + newSection.GetId() + "\", already exists.");
		return;
	}

	ehs::Str_8 newId = newSection.GetId();

	ELF64_Section* shStrTab = FindShStrTab();
	ehs::Serializer<ehs::UInt_64>& data = shStrTab->GetData();

	newSection.nameOffset = data.GetOffset();

	data.Resize(data.Size() + newId.Size() + 1);
	ehs::Util::Copy(&data[data.GetOffset()], &newId[0], newId.Size());
	data.SetOffset(data.GetOffset() + newId.Size());
	data.Write('\0');
	
	sections.Push((ELF64_Section&&)newSection);

	EHS_LOG_SUCCESS();
}

ELF64_Section* ELF64::GetSection(ehs::UInt_64 sectionHashId) const
{
	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
		if (sections[i].GetHashId() == sectionHashId)
			return &sections[i];

	return nullptr;
}

ELF64_Section* ELF64::GetSection(const ehs::Str_8& sectionId) const
{
	return GetSection(sectionId.Hash_64());
}

ehs::UInt_64 ELF64::GetLastSegmentOffset() const
{
	ehs::UInt_64 offset = ELFH_SIZE + PRGH_SIZE * programs.Size();

	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
	{
		if (sections[i].fileOffset)
			offset = sections[i].fileOffset;

		offset += sections[i].GetData().Size();
	}

	return offset;
}

ehs::Vec2_u64 ELF64::SegmentRange(ehs::UInt_64 sectionHashId)
{
	ehs::UInt_64 offset = ELFH_SIZE + PRGH_SIZE * programs.Size();
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

ehs::Vec2_u64 ELF64::SegmentRange(const ehs::Str_8& sectionId)
{
	return SegmentRange(sectionId.Hash_64());
}

ehs::Serializer<ehs::UInt_64> ELF64::Serialize() const
{
	ehs::Serializer<ehs::UInt_64> result(ehs::Endianness::LE);
	result.Resize(Elf_Magic_Size);
	ehs::Util::Copy(&result[0], Elf_Magic, Elf_Magic_Size);
	result.SetOffset(Elf_Magic_Size);

	result.Write<ehs::UInt_8>(ELFH_ARCH_64);
	result.Write(endianness);
	result.Write<ehs::UInt_8>(1);
	result.Write(abi);
	result.Write<ehs::UInt_8>(0);

	result.Resize(result.Size() + 7);
	ehs::Util::Zero(&result[result.GetOffset()], 7);
	result.SetOffset(result.GetOffset() + 7);

	result.Write(type);
	result.Write(arch);
	result.Write<ehs::UInt_32>(1);
	result.Write(programs[entryPoint].pAddr);
	result.Write<ehs::UInt_64>(ELFH_SIZE);
	result.Write(RetrieveSectionsOffset());
	result.Write<ehs::UInt_32>(0);
	result.Write<ehs::UInt_16>(ELFH_SIZE);
	result.Write<ehs::UInt_16>(PRGH_SIZE);
	result.Write(programs.Size());
	result.Write<ehs::UInt_16>(SECH_SIZE);
	result.Write(sections.Size());
	result.Write(FindShStrTabIndex());

	for (ehs::UInt_16 i = 0; i < programs.Size(); i++)
		programs[i].Serialize(result);

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
		sections[i].Serialize(result);
	
	return result;
}

void ELF64::InitializeSections()
{
	ehs::Str_8 nullName = ".NULL";

	ELF64_Section null(nullName, 0, SECH_TYPE_NULL, 0, 0, 0);

	ehs::Str_8 shStrTabName = ".shstrtab";

	ELF64_Section shStrTab(shStrTabName, 0, SECH_TYPE_STRTAB, SECH_FLAG_STRINGS, 0, 1);

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

	sections.Push((ELF64_Section&&)null);
	sections.Push((ELF64_Section&&)shStrTab);
}

ehs::UInt_64 ELF64::RetrieveSectionsOffset() const
{
	ehs::UInt_64 result = ELFH_SIZE + PRGH_SIZE * programs.Size();

	for (ehs::UInt_16 i = 0; i < sections.Size(); i++)
	{
		if (sections[i].fileOffset)
			result = sections[i].fileOffset;

		result += sections[i].GetData().Size();
	}

	return result;
}

ehs::UInt_16 ELF64::FindShStrTabIndex() const
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

ELF64_Section* ELF64::FindShStrTab() const
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
