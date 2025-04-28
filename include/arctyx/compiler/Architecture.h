#pragma once

#include <ehs/Array.h>

#include "Register.h"
#include "Instruction.h"

class EHS_LIB_IO Architecture
{
private:
	friend class Arctyx;

	ehs::UInt_64 id;
	ehs::Str_8 name;
	ehs::Array<Register> registers;
	ehs::Array<Instruction> instructions;

	static ehs::Array<const Architecture *> architectures;

public:
	Architecture();

	Architecture(ehs::Str_8 name);

	Architecture(Architecture &&arch) noexcept;

	Architecture(const Architecture &arc);

	Architecture &operator=(Architecture &&arc) noexcept;

	Architecture &operator=(const Architecture &arc);

	ehs::UInt_64 GetId() const;

	ehs::Str_8 GetName() const;

	bool HasRegister(const ehs::UInt_64 &id) const;

	bool HasRegister(const ehs::Str_8 &name) const;

	const Register *GetRegister(const ehs::UInt_64 &id) const;

	const Register *GetRegister(const ehs::Str_8 &name) const;

	bool AddRegister(Register reg);

	bool HasInstruction(const ehs::UInt_64 &id) const;

	bool HasInstruction(const ehs::Str_8 &name) const;

	const Instruction *GetInstruction(const ehs::UInt_64 &id) const;

	const Instruction *GetInstruction(const ehs::Str_8 &name) const;

	bool AddInstruction(Instruction ins);

	static bool Has(const ehs::UInt_64 &id);

	static bool Has(const ehs::Str_8 &name);

	static const Architecture *Get(const ehs::UInt_64 &id);

	static const Architecture *Get(const ehs::Str_8 &name);

	static bool Add(Architecture arc);
};
