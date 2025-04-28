#pragma once

#include <ehs/Str.h>

class Register
{
private:
    ehs::UInt_64 id;
    ehs::Str_8 name;
    ehs::UInt_32 byteDepth;
    ehs::UInt_64 code;;

public:
    Register();

    Register(ehs::Str_8 name, const ehs::UInt_64 &byteDepth, const ehs::UInt_64 &code);

    Register(Register &&other) noexcept;

    Register(const Register &other);

    Register &operator=(Register &&other) noexcept;

    Register &operator=(const Register &other);

    bool operator==(const ehs::Str_8 &otherName) const;

    bool operator!=(const ehs::Str_8 &otherName) const;

    bool operator==(const ehs::UInt_64 &otherId) const;

    bool operator!=(const ehs::UInt_64 &otherId) const;

    ehs::UInt_64 GetId() const;

    ehs::Str_8 GetName() const;

    ehs::UInt_8 GetByteDepth() const;

    ehs::UInt_64 GetCode() const;
};
