#pragma once

#include "ELF64_Section.h"
#include "ELF64_Program.h"

#include <ehs/Serializer.h>

/// Architecture's memory bit-depth.
#define ELFH_ARCH_32 0x01
#define ELFH_ARCH_64 0x02

/// Architecture's endianness.
#define ELFH_END_LE 0x01
#define ELFH_END_BE 0x02

/// OS' ABI Types.
#define ELFH_ABI_SYSTEMV 0x00

/// ELF Binary Types.
#define ELFH_TYPE_EXEC 0x02
#define ELFH_TYPE_DYN 0x03

/// Machine architectures.
#define ELFH_MARCH_AMD_X86_64 0x003E

/// True header sizes.
#define ELFH_SIZE 64

class ELF64
{
private:
	ehs::UInt_8 endianness;
	ehs::UInt_8 abi;
	ehs::UInt_16 type;
	ehs::UInt_16 arch;
	ehs::UInt_16 entryPoint;
	ehs::Vector<ELF64_Program, ehs::UInt_16> programs;
	ehs::Vector<ELF64_Section, ehs::UInt_16> sections;

public:
	ELF64();

	ELF64(ehs::UInt_8 end, ehs::UInt_8 abi, ehs::UInt_16 type, ehs::UInt_16 arch);

	ELF64(ehs::Serializer<ehs::UInt_64>& data);

	ELF64(ELF64&& header) noexcept;

	ELF64(const ELF64& header);

	ELF64& operator=(ELF64&& header) noexcept;

	ELF64& operator=(const ELF64& header);

	ehs::UInt_8 GetEndianness() const;

	ehs::UInt_8 GetABI() const;

	ehs::UInt_16 GetType() const;

	ehs::UInt_16 GetArchitecture() const;

	void SetEntryPoint(ehs::UInt_16 programIndex);

	ELF64_Program* GetEntryPoint() const;

	void AddProgram(ELF64_Program newProgram);

	bool HasSection(ehs::UInt_64 hashId);

	bool HasSection(ehs::Str_8& id);

	void AddSection(ELF64_Section newSection);

	ELF64_Section* GetSection(ehs::UInt_64 sectionHashId) const;

	ELF64_Section* GetSection(const ehs::Str_8& sectionId) const;

	ehs::UInt_64 GetLastSegmentOffset() const;

	ehs::Vec2_u64 SegmentRange(ehs::UInt_64 sectionHashId);

	ehs::Vec2_u64 SegmentRange(const ehs::Str_8& sectionId);

	ehs::Serializer<ehs::UInt_64> Serialize() const;

private:
	void InitializeSections();

	ehs::UInt_64 RetrieveSectionsOffset() const;

	ehs::UInt_16 FindShStrTabIndex() const;

	ELF64_Section* FindShStrTab() const;
};