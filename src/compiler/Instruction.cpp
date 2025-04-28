#include "arctyx/compiler/Instruction.h"

Instruction::Instruction()
	: arch(nullptr), id(0), translation(nullptr)
{
}

Instruction::Instruction(ehs::Str_8 name)
	: arch(nullptr), id(name.Hash_64()), name((ehs::Str_8 &&)name), translation(nullptr)
{
}

Instruction::Instruction(Instruction &&ins) noexcept
	: arch(ins.arch), id(ins.id), name((ehs::Str_8 &&)ins.name), translation(ins.translation)
{
	ins.arch = nullptr;
	ins.id = 0;
	ins.translation = nullptr;
}

Instruction::Instruction(const Instruction &ins)
	: arch(nullptr), id(ins.id), name(ins.name), translation(ins.translation)
{
}

Instruction &Instruction::operator=(Instruction &&ins) noexcept
{
	if (this == &ins)
		return *this;

	arch = ins.arch;
	id = ins.id;
	name = (ehs::Str_8 &&)ins.name;
	translation = ins.translation;

	ins.arch = nullptr;
	ins.id = 0;
	ins.translation = nullptr;

	return *this;
}

Instruction &Instruction::operator=(const Instruction &ins)
{
	if (this == &ins)
		return *this;

	arch = nullptr;
	id = ins.id;
	name = ins.name;
	translation = ins.translation;

	return *this;
}

const Architecture* Instruction::GetArchitecture() const
{
	return arch;
}

ehs::UInt_64 Instruction::GetId() const
{
	return id;
}

ehs::Str_8 Instruction::GetName() const
{
	return name;
}

void Instruction::SetTranslation(TranslateIns newTranslation)
{
	translation = newTranslation;
}

ehs::Serializer<ehs::UInt_64> Instruction::Translate(const ehs::Byte* data) const
{
	if (!translation)
		return {};

	return translation(this, data);
}
