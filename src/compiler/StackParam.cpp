#include "arctyx/compiler/StackParam.h"

#include <stack>

StackParam::StackParam()
	: reg(nullptr), stackOffset(0), pointer(false)
{
}

StackParam::StackParam(const Register *reg, const bool &pointer)
	: reg(reg), stackOffset(0), pointer(pointer)
{
}

StackParam::StackParam(const ehs::UInt_64 &stackOffset, const bool &pointer)
	: reg(nullptr), stackOffset(stackOffset), pointer(pointer)
{
}

StackParam::StackParam(StackParam&& other) noexcept
	: reg(other.reg), stackOffset(other.stackOffset), pointer(other.pointer)
{
	other.reg = nullptr;
	other.stackOffset = 0;
	other.pointer = false;
}

StackParam::StackParam(const StackParam& other)
	: reg(other.reg), stackOffset(other.stackOffset), pointer(other.pointer)
{
}

StackParam& StackParam::operator=(StackParam&& other) noexcept
{
	if (this == &other)
		return *this;

	reg = other.reg;
	stackOffset = other.stackOffset;
	pointer = other.pointer;

	other.reg = nullptr;
	other.stackOffset = 0;
	other.pointer = false;

	return *this;
}

StackParam& StackParam::operator=(const StackParam& other)
{
	if (this == &other)
		return *this;

	reg = other.reg;
	stackOffset = other.stackOffset;
	pointer = other.pointer;

	return *this;
}

bool StackParam::IsRegister() const
{
	return reg;
}

bool StackParam::IsStack() const
{
	return !reg;
}

bool StackParam::IsPointer() const
{
	return pointer;
}
