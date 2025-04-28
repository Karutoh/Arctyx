#pragma once

#include <cstdint>
#include <ehs/Str.h>

enum class Signedness : ehs::UInt_8
{
    UNSIGNED,
    SIGNED
};

class Primitive
{
private:
    ehs::UInt_64 id;
    ehs::Str_8 name;
    ehs::UInt_8 byteDepth;
    Signedness signedness;

public:
    Primitive();

    Primitive(ehs::Str_8 name, const ehs::UInt_8 &byteDepth, const Signedness &signedness);

    Primitive(Primitive &&other) noexcept;

    Primitive(const Primitive &other);

    Primitive &operator=(Primitive &&other) noexcept;

    Primitive &operator=(const Primitive &other);

    ehs::UInt_64 GetId() const;

    ehs::Str_8 GetName() const;

    ehs::UInt_8 GetByteDepth() const;

    Signedness GetSignedness() const;
};
