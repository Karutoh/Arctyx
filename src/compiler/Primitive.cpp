#include "arctyx/compiler/Primitive.h"

Primitive::Primitive()
    : id(0), byteDepth(0), signedness(Signedness::UNSIGNED)
{
}

Primitive::Primitive(ehs::Str_8 name, const ehs::UInt_8& byteDepth, const Signedness &signedness)
    : id(name.Hash_64()), name((ehs::Str_8 &&)name), byteDepth(byteDepth), signedness(signedness)
{
}

Primitive::Primitive(Primitive&& other) noexcept
    : id(other.id), name((ehs::Str_8 &&)other.name), byteDepth(other.byteDepth), signedness(other.signedness)
{
    other.id = 0;
    other.byteDepth = 0;
    other.signedness = Signedness::UNSIGNED;
}

Primitive::Primitive(const Primitive& other)
    : id(other.id), name(other.name), byteDepth(other.byteDepth), signedness(other.signedness)
{
}

Primitive& Primitive::operator=(Primitive&& other) noexcept
{
    if (this == &other)
        return *this;

    id = other.id;
    name = (ehs::Str_8 &&)other.name;
    byteDepth = other.byteDepth;
    signedness = other.signedness;

    other.id = 0;
    other.byteDepth = 0;
    other.signedness = Signedness::UNSIGNED;

    return *this;
}

Primitive& Primitive::operator=(const Primitive& other)
{
    if (this == &other)
        return *this;

    id = other.id;
    name = other.name;
    byteDepth = other.byteDepth;
    signedness = other.signedness;

    return *this;
}

ehs::UInt_64 Primitive::GetId() const
{
    return id;
}

ehs::Str_8 Primitive::GetName() const
{
    return name;
}

ehs::UInt_8 Primitive::GetByteDepth() const
{
    return byteDepth;
}

Signedness Primitive::GetSignedness() const
{
    return signedness;
}
