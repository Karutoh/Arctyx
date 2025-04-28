#include "arctyx/compiler/Operator.h"

Operator::Operator()
    : instructionId(0)
{
}

Operator::Operator(ehs::Str_8 delimeter, ehs::Str_8 instructionName)
    : unary(delimeter.Size() == 1), delimeter((ehs::Str_8 &&)delimeter), instructionId(instructionName.Hash_64()),
    instructionName((ehs::Str_8 &&)instructionName)
{
}

Operator::Operator(Operator&& other) noexcept
    : unary(other.unary), delimeter((ehs::Str_8 &&)other.delimeter), instructionId(other.instructionId),
    instructionName((ehs::Str_8 &&)other.instructionName)
{
    other.unary = false;
    other.instructionId = 0;
}

Operator::Operator(const Operator& other)
    : unary(other.unary), delimeter(other.delimeter), instructionId(other.instructionId),
    instructionName(other.instructionName)
{
}

Operator& Operator::operator=(Operator&& other) noexcept
{
    if (this == &other)
        return *this;

    unary = other.unary;
    delimeter = (ehs::Str_8 &&)other.delimeter;
    instructionId = other.instructionId;
    instructionName = (ehs::Str_8 &&)other.instructionName;

    other.unary = false;
    other.instructionId = 0;

    return *this;
}

Operator& Operator::operator=(const Operator& other)
{
    if (this == &other)
        return *this;

    unary = other.unary;
    delimeter = other.delimeter;
    instructionId = other.instructionId;
    instructionName = other.instructionName;

    return *this;
}

bool Operator::IsUnary() const
{
    return unary;
}

ehs::Str_8 Operator::GetDelimeter() const
{
    return delimeter;
}

ehs::UInt_64 Operator::GetInstructionId() const
{
    return instructionId;
}

ehs::Str_8 Operator::GetInstructionName() const
{
    return instructionName;
}
