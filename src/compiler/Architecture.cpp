#include "arctyx/compiler/Architecture.h"

ehs::Array<const Architecture *> Architecture::architectures;

Architecture::Architecture()
	: id(0)
{
}

Architecture::Architecture(ehs::Str_8 name)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name)
{
}

Architecture::Architecture(Architecture &&arch) noexcept
	: id(arch.id), name((ehs::Str_8 &&)arch.name), registers((ehs::Array<Register> &&)arch.registers),
	instructions((ehs::Array<Instruction> &&)arch.instructions)
{
	for (ehs::Size i = 0; i < instructions.Size(); ++i)
		instructions[i].arch = this;
}

Architecture::Architecture(const Architecture &arc)
	: id(arc.id), name(arc.name), registers(arc.registers), instructions(arc.instructions)
{
	for (ehs::Size i = 0; i < instructions.Size(); ++i)
		instructions[i].arch = this;
}

Architecture & Architecture::operator=(Architecture &&arc) noexcept
{
	if (this == &arc)
		return *this;

	id = arc.id;
	name = (ehs::Str_8 &&)arc.name;
	registers = (ehs::Array<Register> &&)arc.registers;
	instructions = (ehs::Array<Instruction> &&)arc.instructions;

	for (ehs::Size i = 0; i < instructions.Size(); ++i)
		instructions[i].arch = this;

	arc.id = 0;

	return *this;
}

Architecture & Architecture::operator=(const Architecture &arc)
{
	if (this == &arc)
		return *this;

	id = arc.id;
	name = arc.name;
	registers = arc.registers;
	instructions = arc.instructions;

	for (ehs::Size i = 0; i < instructions.Size(); ++i)
		instructions[i].arch = this;

	return *this;
}

ehs::UInt_64 Architecture::GetId() const
{
	return id;
}

ehs::Str_8 Architecture::GetName() const
{
	return name;
}

bool Architecture::HasRegister(const ehs::UInt_64& id) const
{
	for (ehs::UInt_64 i = 0; i < registers.Size(); ++i)
		if (registers[i].GetId() == id)
			return true;

	return false;
}

bool Architecture::HasRegister(const ehs::Str_8& name) const
{
	return HasRegister(name.Hash_64());
}

const Register* Architecture::GetRegister(const ehs::UInt_64& id) const
{
	for (ehs::UInt_64 i = 0; i < registers.Size(); ++i)
		if (registers[i].GetId() == id)
			return &registers[i];

	return nullptr;
}

const Register* Architecture::GetRegister(const ehs::Str_8& name) const
{
	return GetRegister(name.Hash_64());
}

bool Architecture::AddRegister(Register reg)
{
	if (HasRegister(reg.GetId()))
		return false;

	registers.Push((Register &&)reg);

	return true;
}

bool Architecture::HasInstruction(const ehs::UInt_64 &id) const
{
	for (ehs::UInt_64 i = 0; i < instructions.Size(); ++i)
		if (instructions[i].GetId() == id)
			return true;

	return false;
}

bool Architecture::HasInstruction(const ehs::Str_8 &name) const
{
	return HasInstruction(name.Hash_64());
}

const Instruction *Architecture::GetInstruction(const ehs::UInt_64 &id) const
{
	for (ehs::UInt_64 i = 0; i < instructions.Size(); ++i)
		if (instructions[i].GetId() == id)
			return &instructions[i];

	return nullptr;
}

const Instruction *Architecture::GetInstruction(const ehs::Str_8 &name) const
{
	return GetInstruction(name.Hash_64());
}

bool Architecture::AddInstruction(Instruction ins)
{
	if (HasInstruction(ins.GetId()))
		return false;

	ins.arch = this;

	instructions.Push((Instruction &&)ins);

	return true;
}

bool Architecture::Has(const ehs::UInt_64 &id)
{
	for (ehs::UInt_64 i = 0; i < architectures.Size(); ++i)
		if (architectures[i]->GetId() == id)
			return true;

	return false;
}

bool Architecture::Has(const ehs::Str_8 &name)
{
	return Has(name.Hash_64());
}

const Architecture *Architecture::Get(const ehs::UInt_64 &id)
{
	for (ehs::UInt_64 i = 0; i < architectures.Size(); ++i)
		if (architectures[i]->GetId() == id)
			return architectures[i];

	return nullptr;
}

const Architecture *Architecture::Get(const ehs::Str_8 &name)
{
	return Get(name.Hash_64());
}

bool Architecture::Add(Architecture arc)
{
	if (Has(arc.GetId()))
		return false;

	architectures.Push(new Architecture((Architecture &&)arc));

	return true;
}
