#pragma once

#include <ehs/Serializer.h>

/// Program types.
#define PRGH_TYPE_LOAD 0x00000001

/// Program flags.
#define PRGH_FLAG_EXEC 0x00000001
#define PRGH_FLAG_WRITE 0x00000002
#define PRGH_FLAG_READ 0x00000004

#define PRGH_SIZE 56

class ELF64;

class ELF64_Program
{
private:
	friend class ELF64;

	ehs::UInt_32 type;
	ehs::UInt_32 flags;
	ehs::UInt_64 offset;
	ehs::UInt_64 vAddr;
	ehs::UInt_64 pAddr;
	ehs::UInt_64 fileSize;
	ehs::UInt_64 adjustedMemSize;
	ehs::UInt_64 align;

public:
	ELF64_Program();

	ELF64_Program(ehs::UInt_32 type, ehs::UInt_32 flags, ehs::UInt_64 offset, ehs::UInt_64 address, ehs::UInt_64 size, ehs::UInt_64 adjustedMemSize, ehs::UInt_64 align);

	ELF64_Program(ehs::Serializer<ehs::UInt_64>& data);

	ELF64_Program(ELF64_Program&& program) noexcept;

	ELF64_Program(const ELF64_Program& program);

	ELF64_Program& operator=(ELF64_Program&& program) noexcept;

	ELF64_Program& operator=(const ELF64_Program& program);

	ehs::UInt_32 GetType() const;

	ehs::UInt_32 GetFlags() const;

	ehs::UInt_64 GetAlign() const;

	void Serialize(ehs::Serializer<ehs::UInt_64>& inData) const;
};