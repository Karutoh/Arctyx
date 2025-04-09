#pragma once

#include <ehs/Serializer.h>

/// Program types.
#define PRGH_TYPE_LOAD 0x00000001

/// Program flags.
#define PRGH_FLAG_EXEC 0x00000001
#define PRGH_FLAG_WRITE 0x00000002
#define PRGH_FLAG_READ 0x00000004

#define PRGH_SIZE 56

class ELF;

class ELF_Program
{
private:
	friend class ELF;

	ehs::UInt_32 type;
	ehs::UInt_32 flags;
	ehs::UInt_64 offset;
	ehs::UInt_64 vAddr;
	ehs::UInt_64 pAddr;
	ehs::UInt_64 fileSize;
	ehs::UInt_64 adjustedMemSize;
	ehs::UInt_64 align;

public:
	ELF_Program();

	ELF_Program(ehs::UInt_32 type, ehs::UInt_32 flags, ehs::UInt_64 offset, ehs::UInt_64 address, ehs::UInt_64 size, ehs::UInt_64 adjustedMemSize, ehs::UInt_64 align);

	ELF_Program(ehs::Serializer<ehs::UInt_64>& data);

	ELF_Program(ELF_Program&& program) noexcept;

	ELF_Program(const ELF_Program& program);

	ELF_Program& operator=(ELF_Program&& program) noexcept;

	ELF_Program& operator=(const ELF_Program& program);

	ehs::UInt_32 GetType() const;

	ehs::UInt_32 GetFlags() const;

	ehs::UInt_64 GetAlign() const;

	void Serialize(ehs::Serializer<ehs::UInt_64>& inData) const;
};