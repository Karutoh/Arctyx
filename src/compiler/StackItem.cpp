#include "arctyx/compiler/StackItem.h"

StackItem::StackItem()
    : id(0), address(0), size(0)
{
}

StackItem::StackItem(ehs::Str_8 name, const ehs::UInt_64& address, const ehs::UInt_64 &size)
    : id(name.Hash_64()), name((ehs::Str_8 &&)name), address(address), size(size)
{
}

StackItem::StackItem(StackItem&& other) noexcept
    : id(other.id), name((ehs::Str_8 &&)other.name), address(other.address), size(other.size)
{
    other.id = 0;
    other.address = 0;
    other.size = 0;
}

StackItem::StackItem(const StackItem &other)
    : id(other.id), name(other.name), address(other.address), size(other.size)
{
}

StackItem &StackItem::operator=(StackItem &&other) noexcept
{
    if (this == &other)
        return *this;

    name = (ehs::Str_8 &&)other.name;
    id = other.id;
    address = other.address;
    size = other.size;

    other.id = 0;
    other.address = 0;
    other.size = 0;

    return *this;
}

StackItem &StackItem::operator=(const StackItem& other)
{
    if (this == &other)
        return *this;

    name = other.name;
    id = other.id;
    address = other.address;
    size = other.size;

    return *this;
}

ehs::Str_8 StackItem::GetName() const
{
    return name;
}

ehs::UInt_64 StackItem::GetId() const
{
    return id;
}

ehs::UInt_64 StackItem::GetAddress() const
{
    return address;
}

ehs::UInt_64 StackItem::GetSize() const
{
    return size;
}