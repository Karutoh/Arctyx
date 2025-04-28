#pragma once
#include <ehs/Str.h>
#include <ehs/Types.h>

class Operator
{
private:
    bool unary;
    ehs::Str_8 delimeter;
    ehs::UInt_64 instructionId;
    ehs::Str_8 instructionName;

public:
    Operator();

    Operator(ehs::Str_8 delimeter, ehs::Str_8 instructionName);

    Operator(Operator &&other) noexcept;

    Operator(const Operator &other);

    Operator &operator=(Operator &&other) noexcept;

    Operator &operator=(const Operator &other);

    bool IsUnary() const;

    ehs::Str_8 GetDelimeter() const;

    ehs::UInt_64 GetInstructionId() const;

    ehs::Str_8 GetInstructionName() const;
};
