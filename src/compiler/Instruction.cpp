#include "arctyx/compiler/Instruction.h"

Instruction::~Instruction()
{
	delete code;
}

Instruction::Instruction()
	: id(0), size(0), code(nullptr)
{
}

Instruction::Instruction(ehs::Str_8 name, const ehs::UInt_8 &size, ehs::Byte *code)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), size(size), code(code)
{
}

Instruction::Instruction(ehs::Str_8 name, const ehs::UInt_64 &code)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), size(sizeof(code)), code(new ehs::Byte[size])
{
	*(ehs::UInt_64 *)this->code = code;
}

Instruction::Instruction(ehs::Str_8 name, const ehs::UInt_32 &code)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), size(sizeof(code)), code(new ehs::Byte[size])
{
	*(ehs::UInt_32 *)this->code = code;
}

Instruction::Instruction(ehs::Str_8 name, const ehs::UInt_16 &code)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), size(sizeof(code)), code(new ehs::Byte[size])
{
	*(ehs::UInt_16 *)this->code = code;
}

Instruction::Instruction(ehs::Str_8 name, const ehs::UInt_8 &code)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), size(sizeof(code)), code(new ehs::Byte[size])
{
	*this->code = code;
}

Instruction::Instruction(Instruction &&ins) noexcept
	: id(ins.id), name((ehs::Str_8 &&)ins.name), size(ins.size), code(ins.code)
{
	ins.id = 0;
	ins.size = 0;
	ins.code = nullptr;
}

Instruction::Instruction(const Instruction &ins)
	: id(ins.id), name(ins.name), size(ins.size), code(new ehs::Byte[size])
{
	ehs::Util::Copy(code, ins.code, size);
}

Instruction &Instruction::operator=(Instruction &&ins) noexcept
{
	if (this == &ins)
		return *this;

	id = ins.id;
	name = (ehs::Str_8 &&)ins.name;
	size = ins.size;
	code = ins.code;

	ins.id = 0;
	ins.size = 0;
	ins.code = nullptr;

	return *this;
}

Instruction &Instruction::operator=(const Instruction &ins)
{
	if (this == &ins)
		return *this;

	id = ins.id;
	name = ins.name;
	size = ins.size;
	code = new ehs::Byte[size];

	ehs::Util::Copy(code, ins.code, size);

	return *this;
}

ehs::UInt_64 Instruction::GetId() const
{
	return id;
}

ehs::Str_8 Instruction::GetName() const
{
	return name;
}

ehs::UInt_8 Instruction::GetSize() const
{
	return size;
}

ehs::Byte *Instruction::GetCode() const
{
	return code;
}
