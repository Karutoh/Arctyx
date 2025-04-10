#pragma once

#include <ehs/Array.h>

#include "Instruction.h"

class Architecture
{
private:
	ehs::UInt_64 id;
	ehs::Str_8 name;
	ehs::Array<Instruction> instructions;

public:
	Architecture();

	Architecture(ehs::Str_8 name);

	Architecture(Architecture &&arch) noexcept;

	Architecture(const Architecture &arc);

	Architecture &operator=(Architecture &&arc) noexcept;

	Architecture &operator=(const Architecture &arc);

	ehs::UInt_64 Id() const;

	ehs::Str_8 GetName() const;

	bool HasInstruction(const ehs::UInt_64 &id) const;

	bool HasInstruction(const ehs::Str_8 &name) const;

	const Instruction *GetInstruction(const ehs::UInt_64 &id) const;

	const Instruction *GetInstruction(const ehs::Str_8 &name) const;

	bool AddInstruction(Instruction ins);
};