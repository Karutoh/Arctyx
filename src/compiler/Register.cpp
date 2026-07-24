#include "arctyx/compiler/Register.h"

Register::Register()
    : id(0), byteDepth(0), code(0), data(0)
{
}

Register::Register(ehs::Str_8 name, const ehs::UInt_64 &byteDepth, const ehs::UInt_64 &code)
    : id(name.Hash_64()), name((ehs::Str_8 &&)name), byteDepth(byteDepth), code(code), data(0)
{
}

Register::Register(Register&& other) noexcept
    : id(other.id), name((ehs::Str_8 &&)other.name), byteDepth(other.byteDepth), code(other.code), data(other.data)
{
    other.id = 0;
    other.byteDepth = 0;
    other.code = 0;
	other.data = 0;
}

Register::Register(const Register& other)
    : id(other.id), name(other.name), byteDepth(other.byteDepth), code(other.code), data(other.data)
{
}

Register& Register::operator=(Register&& other) noexcept
{
    if (this == &other)
        return *this;

    id = other.id;
    name = (ehs::Str_8 &&)other.name;
    byteDepth = other.byteDepth;
    code = other.code;
	data = other.data;

    other.id = 0;
    other.byteDepth = 0;
    other.code = 0;
	other.data = 0;

    return *this;
}

Register& Register::operator=(const Register& other)
{
    if (this == &other)
        return *this;

    id = other.id;
    name = other.name;
    byteDepth = other.byteDepth;
    code = other.code;
	data = other.data;

    return *this;
}

bool Register::operator==(const ehs::Str_8& otherName) const
{
    return name == otherName;
}

bool Register::operator!=(const ehs::Str_8& otherName) const
{
    return name != otherName;
}

bool Register::operator==(const ehs::UInt_64& otherId) const
{
    return id == otherId;
}

bool Register::operator!=(const ehs::UInt_64& otherId) const
{
    return id != otherId;
}

ehs::UInt_64 Register::GetId() const
{
    return id;
}

ehs::Str_8 Register::GetName() const
{
    return name;
}

ehs::UInt_8 Register::GetByteDepth() const
{
    return byteDepth;
}

ehs::UInt_64 Register::GetCode() const
{
    return code;
}

void Register::SetData(const ehs::UInt_64& newData)
{
	data = newData;
}

ehs::UInt_64 Register::GetData() const
{
	return data;
}
