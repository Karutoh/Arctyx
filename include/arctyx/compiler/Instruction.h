#pragma once
#include <ehs/Str.h>
#include <ehs/Types.h>

class Instruction
{
private:
	ehs::UInt_64 id;
	ehs::Str_8 name;
	ehs::UInt_8 size;
	ehs::Byte *code;

public:
	~Instruction();

	Instruction();

	Instruction(ehs::Str_8 name, const ehs::UInt_8 &size, ehs::Byte *code);

	Instruction(ehs::Str_8 name, const ehs::UInt_64 &code);

	Instruction(ehs::Str_8 name, const ehs::UInt_32 &code);

	Instruction(ehs::Str_8 name, const ehs::UInt_16 &code);

	Instruction(ehs::Str_8 name, const ehs::UInt_8 &code);

	Instruction(Instruction &&ins) noexcept;

	Instruction(const Instruction &ins);

	Instruction &operator=(Instruction &&ins) noexcept;

	Instruction &operator=(const Instruction &ins);

	ehs::UInt_64 GetId() const;

	ehs::Str_8 GetName() const;

	ehs::UInt_8 GetSize() const;

	ehs::Byte *GetCode() const;
};
