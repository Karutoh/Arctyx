#include "arctyx/linker/formats/ELF_Program.h"
#include "arctyx/linker/formats/ELF_Architecture.h"

ELF_Program::ELF_Program()
	: type(PRGH_TYPE_LOAD), flags(PRGH_FLAG_EXEC | PRGH_FLAG_READ), offset(0), vAddr(0), pAddr(0), fileSize(0),
	adjustedMemSize(0), align(0x200000)
{
}

ELF_Program::ELF_Program(ehs::UInt_32 type, ehs::UInt_32 flags, ehs::UInt_64 offset, ehs::UInt_64 address, ehs::UInt_64 size, ehs::UInt_64 adjustedMemSize, ehs::UInt_64 align)
	: type(type), flags(flags), offset(offset), vAddr(address), pAddr(address), fileSize(size),
	adjustedMemSize(adjustedMemSize), align(align)
{
}

ELF_Program::ELF_Program(ehs::Serializer<ehs::UInt_64>& data)
	: type(PRGH_TYPE_LOAD), flags(PRGH_FLAG_EXEC | PRGH_FLAG_READ), offset(0), vAddr(0), pAddr(0), fileSize(0),
	adjustedMemSize(0), align(0x200000)
{
}

ELF_Program::ELF_Program(ELF_Program&& program) noexcept
	: type(program.type), flags(program.flags), offset(program.offset), vAddr(program.vAddr), pAddr(program.pAddr),
	fileSize(program.fileSize), adjustedMemSize(program.adjustedMemSize), align(program.align)
{
	program.type = PRGH_TYPE_LOAD;
	program.flags = PRGH_FLAG_EXEC | PRGH_FLAG_READ;
	program.offset = 0;
	program.vAddr = 0;
	program.pAddr = 0;
	program.fileSize = 0;
	program.adjustedMemSize = 0;
	program.align = 0x200000;
}

ELF_Program::ELF_Program(const ELF_Program& program)
	: type(program.type), flags(program.flags), offset(program.offset), vAddr(program.vAddr), pAddr(program.pAddr),
	fileSize(program.fileSize), adjustedMemSize(program.adjustedMemSize), align(program.align)
{
}

ELF_Program& ELF_Program::operator=(ELF_Program&& program) noexcept
{
	if (this == &program)
		return *this;

	type = program.type;
	flags = program.flags;
	offset = program.offset;
	vAddr = program.vAddr;
	pAddr = program.pAddr;
	fileSize = program.fileSize;
	adjustedMemSize = program.adjustedMemSize;
	align = program.align;

	program.type = PRGH_TYPE_LOAD;
	program.flags = PRGH_FLAG_EXEC | PRGH_FLAG_READ;
	program.offset = 0;
	program.vAddr = 0;
	program.pAddr = 0;
	program.fileSize = 0;
	program.adjustedMemSize = 0;
	program.align = 0x200000;

	return *this;
}

ELF_Program& ELF_Program::operator=(const ELF_Program& program)
{
	if (this == &program)
		return *this;

	type = program.type;
	flags = program.flags;
	offset = program.offset;
	vAddr = program.vAddr;
	pAddr = program.pAddr;
	fileSize = program.fileSize;
	adjustedMemSize = program.adjustedMemSize;
	align = program.align;

	return *this;
}

ehs::UInt_32 ELF_Program::GetType() const
{
	return type;
}

ehs::UInt_32 ELF_Program::GetFlags() const
{
	return flags;
}

ehs::UInt_64 ELF_Program::GetAlign() const
{
	return align;
}

void ELF_Program::Serialize(const ehs::UInt_8 &bitDepth, ehs::Serializer<ehs::UInt_64>& inData) const
{
	if (bitDepth == ELF_BIT_DEPTH_64)
		Serialize_64(inData);
	else if (bitDepth == ELF_BIT_DEPTH_32)
		Serialize_32(inData);
}

void ELF_Program::Serialize_64(ehs::Serializer<ehs::UInt_64> &data) const
{
	data.Write(type);
	data.Write(flags);
	data.Write(offset);
	data.Write(vAddr);
	data.Write(pAddr);
	data.Write(fileSize);
	data.Write(fileSize + adjustedMemSize);
	data.Write(align);
}

void ELF_Program::Serialize_32(ehs::Serializer<ehs::UInt_64> &data) const
{
	data.Write(type);
	data.Write<ehs::UInt_32>(offset);
	data.Write<ehs::UInt_32>(vAddr);
	data.Write<ehs::UInt_32>(pAddr);
	data.Write<ehs::UInt_32>(fileSize);
	data.Write<ehs::UInt_32>(fileSize + adjustedMemSize);
	data.Write(flags);
	data.Write<ehs::UInt_32>(align);
}
