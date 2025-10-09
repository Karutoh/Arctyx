#pragma once

#include "arctyx/compiler/Register.h"

class StackParam
{
private:
    const Register *reg;
    ehs::UInt_64 stackOffset;
	bool pointer;

public:
    StackParam();

    StackParam(const Register *reg, bool pointer);

    StackParam(ehs::UInt_64 stackOffset, bool pointer);

	StackParam(StackParam &&other) noexcept;

	StackParam(const StackParam &other);

	StackParam &operator=(StackParam &&other) noexcept;

	StackParam &operator=(const StackParam &other);

	bool IsRegister() const;

	bool IsStack() const;

	bool IsPointer() const;
};