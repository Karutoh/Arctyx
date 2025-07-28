#pragma once

#include <ehs/Str.h>

class StackItem
{
private:
    ehs::UInt_64 id;
    ehs::Str_8 name;
    ehs::UInt_64 address;
    ehs::UInt_64 size;

public:
    StackItem();

    StackItem(ehs::Str_8 name, const ehs::UInt_64 &address, const ehs::UInt_64 &size);

    StackItem(StackItem &&other) noexcept;

    StackItem(const StackItem &other);

    StackItem &operator=(StackItem &&other) noexcept;

    StackItem &operator=(const StackItem &other);

    ehs::Str_8 GetName() const;

    ehs::UInt_64 GetId() const;

    ehs::UInt_64 GetAddress() const;

    ehs::UInt_64 GetSize() const;
};