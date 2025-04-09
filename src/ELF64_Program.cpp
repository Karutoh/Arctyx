#include "ELF64_Program.h"

ELF64_Program::ELF64_Program()
	: type(PRGH_TYPE_LOAD), flags(PRGH_FLAG_EXEC | PRGH_FLAG_READ), offset(0), vAddr(0), pAddr(0), fileSize(0),
	adjustedMemSize(0), align(0x200000)
{
}

ELF64_Program::ELF64_Program(ehs::UInt_32 type, ehs::UInt_32 flags, ehs::UInt_64 offset, ehs::UInt_64 address, ehs::UInt_64 size, ehs::UInt_64 adjustedMemSize, ehs::UInt_64 align)
	: type(type), flags(flags), offset(offset), vAddr(address), pAddr(address), fileSize(size),
	adjustedMemSize(adjustedMemSize), align(align)
{
}

ELF64_Program::ELF64_Program(ehs::Serializer<ehs::UInt_64>& data)
	: type(PRGH_TYPE_LOAD), flags(PRGH_FLAG_EXEC | PRGH_FLAG_READ), offset(0), vAddr(0), pAddr(0), fileSize(0),
	adjustedMemSize(0), align(0x200000)
{
}

ELF64_Program::ELF64_Program(ELF64_Program&& program) noexcept
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

ELF64_Program::ELF64_Program(const ELF64_Program& program)
	: type(program.type), flags(program.flags), offset(program.offset), vAddr(program.vAddr), pAddr(program.pAddr),
	fileSize(program.fileSize), adjustedMemSize(program.adjustedMemSize), align(program.align)
{
}

ELF64_Program& ELF64_Program::operator=(ELF64_Program&& program) noexcept
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

ELF64_Program& ELF64_Program::operator=(const ELF64_Program& program)
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

ehs::UInt_32 ELF64_Program::GetType() const
{
	return type;
}

ehs::UInt_32 ELF64_Program::GetFlags() const
{
	return flags;
}

ehs::UInt_64 ELF64_Program::GetAlign() const
{
	return align;
}

void ELF64_Program::Serialize(ehs::Serializer<ehs::UInt_64>& inData) const
{
	inData.Write(type);
	inData.Write(flags);
	inData.Write(offset);
	inData.Write(vAddr);
	inData.Write(pAddr);
	inData.Write(fileSize);
	inData.Write(fileSize + adjustedMemSize);
	inData.Write(align);
}
