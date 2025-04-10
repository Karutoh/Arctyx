#pragma once

#include "ELF_Architecture.h"
#include "ELF_Section.h"
#include "ELF_Program.h"

#include <ehs/Serializer.h>

/// OS' ABI Types.
#define ELFH_ABI_SYSTEMV 0x00

/// ELF Binary Types.
#define ELFH_TYPE_EXEC 0x02
#define ELFH_TYPE_DYN 0x03

/// True header sizes.
#define ELF_64_BIT_HEADER_SIZE 64
#define ELF_32_BIT_HEADER_SIZE 52

class ELF
{
private:
	ehs::UInt_8 bitDepth;
	ehs::UInt_8 endianness;
	ehs::UInt_8 abi;
	ehs::UInt_16 type;
	ehs::UInt_16 arch;
	ehs::UInt_16 entryPoint;
	ehs::Vector<ELF_Program, ehs::UInt_16> programs;
	ehs::Vector<ELF_Section, ehs::UInt_16> sections;

public:
	ELF();

	ELF(const ehs::UInt_8 &bitDepth, const ehs::UInt_8 &end, const ehs::UInt_8 &abi, const ehs::UInt_16 &type, const ehs::UInt_16 &arch);

	ELF(ehs::Serializer<ehs::UInt_64>& data);

	ELF(ELF&& header) noexcept;

	ELF(const ELF& header);

	ELF& operator=(ELF&& header) noexcept;

	ELF& operator=(const ELF& header);

	ehs::UInt_8 GetBitDepth() const;

	ehs::UInt_8 GetEndianness() const;

	ehs::UInt_8 GetABI() const;

	ehs::UInt_16 GetType() const;

	ehs::UInt_16 GetArchitecture() const;

	void SetEntryPoint(ehs::UInt_16 programIndex);

	ELF_Program* GetEntryPoint() const;

	void AddProgram(ELF_Program newProgram);

	bool HasSection(ehs::UInt_64 hashId);

	bool HasSection(ehs::Str_8& id);

	void AddSection(ELF_Section newSection);

	ELF_Section* GetSection(ehs::UInt_64 sectionHashId) const;

	ELF_Section* GetSection(const ehs::Str_8& sectionId) const;

	ehs::UInt_64 GetLastSegmentOffset() const;

	ehs::Vec2_u64 SegmentRange(ehs::UInt_64 sectionHashId);

	ehs::Vec2_u64 SegmentRange(const ehs::Str_8& sectionId);

	ehs::Serializer<ehs::UInt_64> Serialize() const;

private:
	void Serialize_64(ehs::Serializer<ehs::UInt_64> &data) const;

	void Serialize_32(ehs::Serializer<ehs::UInt_64> &data) const;

	void InitializeSections();

	ehs::UInt_64 RetrieveSectionsOffset() const;

	ehs::UInt_16 FindShStrTabIndex() const;

	ELF_Section* FindShStrTab() const;
};
