#include "arctyx/ELF_Section.h"

ELF_Section::ELF_Section()
	: hashId(0), nameOffset(0), fileOffset(0), type(0), flags(0), vAddr(0), segmentOffset(0), link(0),
	info(0), align(0), entries(0), data(ehs::Endianness::LE)
{
}

ELF_Section::ELF_Section(ehs::Str_8 id, ehs::UInt_64 fileOffset, ehs::UInt_32 type, ehs::UInt_64 flags, ehs::UInt_64 vAddr, ehs::UInt_64 align)
	: hashId(id.Hash_64()), id((ehs::Str_8&&)id), nameOffset(0), fileOffset(fileOffset), type(type), flags(flags), vAddr(vAddr),
	segmentOffset(0), link(0), info(0), align(align), entries(0), data(ehs::Endianness::LE)
{

}

ELF_Section::ELF_Section(ELF_Section&& sect) noexcept
	: hashId(sect.hashId), id((ehs::Str_8&&)sect.id), nameOffset(sect.nameOffset), fileOffset(sect.fileOffset),
	type(sect.type), flags(sect.flags), vAddr(sect.vAddr), segmentOffset(sect.segmentOffset),
	  link(sect.link), info(sect.info), align(sect.align), entries(sect.entries),
	data((ehs::Serializer<ehs::UInt_64>&&)sect.data)
{
	sect.hashId = 0;
	sect.nameOffset = 0;
	sect.fileOffset = 0;
	sect.type = 0;
	sect.flags = 0;
	sect.vAddr = 0;
	sect.segmentOffset = 0;
	sect.link = 0;
	sect.info = 0;
	sect.align = 0;
	sect.entries = 0;
}

ELF_Section::ELF_Section(const ELF_Section& sect)
	: hashId(sect.hashId), id(sect.id), nameOffset(0), fileOffset(sect.fileOffset), type(sect.type),
	flags(sect.flags), vAddr(sect.vAddr), segmentOffset(sect.segmentOffset), link(sect.link),
	info(sect.info), align(sect.align), entries(sect.entries), data(sect.data)
{
}

ELF_Section& ELF_Section::operator=(ELF_Section&& sect) noexcept
{
	if (this == &sect)
		return *this;

	hashId = sect.hashId;
	id = (ehs::Str_8&&)sect.id;
	nameOffset = sect.nameOffset;
	fileOffset = sect.fileOffset;
	type = sect.type;
	flags = sect.flags;
	vAddr = sect.vAddr;
	segmentOffset = sect.segmentOffset;
	link = sect.link;
	info = sect.info;
	align = sect.align;
	entries = sect.entries;
	data = (ehs::Serializer<ehs::UInt_64>&&)sect.data;

	sect.hashId = 0;
	sect.nameOffset = 0;
	sect.fileOffset = 0;
	sect.type = 0;
	sect.flags = 0;
	sect.vAddr = 0;
	sect.segmentOffset = 0;
	sect.link = 0;
	sect.info = 0;
	sect.align = 0;
	sect.entries = 0;
	sect.data = {ehs::Endianness::LE};
	
	return *this;
}

ELF_Section& ELF_Section::operator=(const ELF_Section& sect)
{
	if (this == &sect)
		return *this;

	hashId = sect.hashId;
	id = sect.id;
	nameOffset = 0;
	fileOffset = sect.fileOffset;
	type = sect.type;
	flags = sect.flags;
	vAddr = sect.vAddr;
	segmentOffset = sect.segmentOffset;
	link = sect.link;
	info = sect.info;
	align = sect.align;
	entries = sect.entries;
	data = sect.data;

	return *this;
}

ehs::UInt_64 ELF_Section::GetHashId() const
{
	return hashId;
}

ehs::Str_8 ELF_Section::GetId() const
{
	return id;
}

ehs::UInt_32 ELF_Section::GetType() const
{
	return type;
}

ehs::UInt_64 ELF_Section::GetFlags() const
{
	return flags;
}

ehs::UInt_64 ELF_Section::GetVirtualAddress() const
{
	return vAddr;
}

void ELF_Section::SetLink(ehs::UInt_32 newLink)
{
	link = newLink;
}

void ELF_Section::SetInfo(ehs::UInt_32 newInfo)
{
	info = newInfo;
}

ehs::UInt_64 ELF_Section::GetAlignment() const
{
	return align;
}

void ELF_Section::SetEntries(ehs::UInt_64 newEntries)
{
	entries = newEntries;
}

void ELF_Section::SetData(ehs::Serializer<ehs::UInt_64> newData)
{
	data = (ehs::Serializer<ehs::UInt_64>&&)newData;
}

ehs::Serializer<ehs::UInt_64>& ELF_Section::GetData()
{
	return data;
}

void ELF_Section::Serialize(ehs::Serializer<ehs::UInt_64>& inData) const
{
	inData.Write(nameOffset);
	inData.Write(type);
	inData.Write(flags);
	inData.Write(vAddr);
	inData.Write(segmentOffset);
	inData.Write(data.Size());
	inData.Write(link);
	inData.Write(info);
	inData.Write(align);
	inData.Write(entries);
}
