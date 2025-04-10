#include "arctyx/compiler/Architecture.h"

Architecture::Architecture()
	: id(0)
{
}

Architecture::Architecture(ehs::Str_8 name)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name)
{
}

Architecture::Architecture(Architecture &&arch) noexcept
	: id(arch.id), name((ehs::Str_8 &&)arch.name)
{
}

Architecture::Architecture(const Architecture &arc)
	: id(arc.id), name(arc.name)
{
}

Architecture & Architecture::operator=(Architecture &&arc) noexcept
{
	if (this == &arc)
		return *this;

	id = arc.id;
	name = (ehs::Str_8 &&)arc.name;

	arc.id = 0;

	return *this;
}

Architecture & Architecture::operator=(const Architecture &arc)
{
	if (this == &arc)
		return *this;

	id = arc.id;
	name = arc.name;

	return *this;
}

ehs::UInt_64 Architecture::Id() const
{
	return id;
}

ehs::Str_8 Architecture::GetName() const
{
	return name;
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

	instructions.Push((Instruction &&)ins);

	return true;
}
