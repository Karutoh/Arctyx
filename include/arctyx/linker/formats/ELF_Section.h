#pragma once

#include <ehs/Serializer.h>
#include <ehs/Array.h>
#include <ehs/Str.h>

/// Section types.
#define SECH_TYPE_NULL 0x0000
#define SECH_TYPE_PROGBITS 0x0001
#define SECH_TYPE_SYMTAB 0x0002
#define SECH_TYPE_STRTAB 0x0003

/// Section flags.
#define SECH_FLAG_ALLOC 0x02
#define SECH_FLAG_EXEC 0x04
#define SECH_FLAG_STRINGS 0x20

#define SECH_SIZE 64

class ELF_Section
{
private:
	friend class ELF;

	ehs::UInt_64 hashId;
	ehs::Str_8 id;
	ehs::UInt_32 nameOffset;
	ehs::UInt_64 fileOffset;
	ehs::UInt_32 type;
	ehs::UInt_64 flags;
	ehs::UInt_64 vAddr;
	ehs::UInt_64 segmentOffset;
	ehs::UInt_32 link;
	ehs::UInt_32 info;
	ehs::UInt_64 align;
	ehs::UInt_64 entries;
	ehs::Serializer<ehs::UInt_64> data;

public:
	ELF_Section();

	ELF_Section(ehs::Str_8 id, ehs::UInt_64 fileOffset, ehs::UInt_32 type, ehs::UInt_64 flags, ehs::UInt_64 vAddr, ehs::UInt_64 align);

	ELF_Section(ELF_Section&& sect) noexcept;

	ELF_Section(const ELF_Section& sect);

	ELF_Section& operator=(ELF_Section&& sect) noexcept;

	ELF_Section& operator=(const ELF_Section& sect);

	ehs::UInt_64 GetHashId() const;

	ehs::Str_8 GetId() const;

	ehs::UInt_32 GetType() const;

	ehs::UInt_64 GetFlags() const;

	ehs::UInt_64 GetVirtualAddress() const;

	void SetLink(ehs::UInt_32 newLink);

	void SetInfo(ehs::UInt_32 newInfo);

	ehs::UInt_64 GetAlignment() const;

	void SetEntries(ehs::UInt_64 newEntries);

	void SetData(ehs::Serializer<ehs::UInt_64> newData);

	ehs::Serializer<ehs::UInt_64>& GetData();

	void Serialize(const ehs::UInt_8 &bitDepth, ehs::Serializer<ehs::UInt_64>& inData) const;

private:
	void Serialize_64(ehs::Serializer<ehs::UInt_64> &data) const;

	void Serialize_32(ehs::Serializer<ehs::UInt_64> &data) const;
};