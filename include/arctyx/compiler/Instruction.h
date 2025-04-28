#pragma once

#include <ehs/Array.h>
#include <ehs/Serializer.h>
#include <ehs/Str.h>
#include <ehs/Types.h>

#include "Token.h"

class Instruction;
class Architecture;
class Compiler;
class Language;

typedef ehs::Serializer<ehs::UInt_64> (*TranslateIns)(const Instruction *, const ehs::Byte *);

class EHS_LIB_IO Instruction
{
private:
	friend class Architecture;

	Architecture *arch;
	ehs::UInt_64 id;
	ehs::Str_8 name;
	TranslateIns translation;

public:
	Instruction();

	Instruction(ehs::Str_8 name);

	Instruction(Instruction &&ins) noexcept;

	Instruction(const Instruction &ins);

	Instruction &operator=(Instruction &&ins) noexcept;

	Instruction &operator=(const Instruction &ins);

	const Architecture *GetArchitecture() const;

	ehs::UInt_64 GetId() const;

	ehs::Str_8 GetName() const;

	void SetTranslation(TranslateIns newTranslation);

	ehs::Serializer<ehs::UInt_64> Translate(const ehs::Byte *data) const;
};

struct AssignBase
{
	const ehs::UInt_8 type = 0;
};

struct AssignRR
{
	AssignBase base = {
		1
	};
	bool isDstAddress = false;
	ehs::UInt_64 dstReg = 0;
	bool isSrcAddress = false;
	ehs::UInt_64 srcReg = 0;
};

struct AssignRI
{
	AssignBase base = {
		2
	};
	ehs::UInt_64 dstReg = 0;
	ehs::UInt_64 srcImm = 0;
};