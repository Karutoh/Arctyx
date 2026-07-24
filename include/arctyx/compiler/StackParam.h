#pragma once

#include "arctyx/compiler/Register.h"

class StackParam
{
private:
    const Register *reg;
    ehs::UInt_64 stackOffset;
	ehs::UInt_8 size;
	bool pointer;

public:
    StackParam();

    StackParam(const Register *reg, const bool &pointer);

    StackParam(const ehs::UInt_64 &stackOffset, const bool &pointer);

	StackParam(StackParam &&other) noexcept;

	StackParam(const StackParam &other);

	StackParam &operator=(StackParam &&other) noexcept;

	StackParam &operator=(const StackParam &other);

	bool IsRegister() const;

	bool IsStack() const;

	bool IsPointer() const;
};