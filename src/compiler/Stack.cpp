#include "arctyx/compiler/Stack.h"

Stack::Stack()
{
}

Stack::Stack(const ehs::UInt_64& offset)
{
}

Stack::Stack(Stack&& other) noexcept
{
}

Stack::Stack(const Stack& other)
{
}

Stack& Stack::operator=(Stack&& other) noexcept
{
}

Stack& Stack::operator=(const Stack& other)
{
}

ehs::UInt_64 Stack::GetOffset() const
{
}

bool Stack::HasItem(const ehs::UInt_64& id) const
{
}

bool Stack::HasItem(const ehs::Str_8& name) const
{
}

bool Stack::AddItem(StackItem item)
{
}

StackItem* Stack::GetItem(const ehs::UInt_64& id) const
{
}

StackItem* Stack::GetItem(const ehs::Str_8& name) const
{
}
