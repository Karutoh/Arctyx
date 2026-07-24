#pragma once

#include <ehs/Array.h>
#include <ehs/Vector.h>

#include "arctyx/compiler/StackParam.h"
#include "arctyx/compiler/StackItem.h"

class Stack
{
private:
    ehs::Array<StackParam> inputs;
    ehs::Array<StackParam> outputs;
    ehs::UInt_64 offset;
    ehs::Vector<StackItem> items;

public:
    Stack();

    Stack(const ehs::UInt_64 &offset);

    Stack(Stack &&other) noexcept;

    Stack(const Stack &other);

    Stack &operator=(Stack &&other) noexcept;

    Stack &operator=(const Stack &other);

    ehs::UInt_64 GetOffset() const;

	bool HasItem(const ehs::UInt_64 &id) const;

	bool HasItem(const ehs::Str_8 &name) const;

    void AddItem(StackItem item);

	bool RemoveItem(const ehs::Str_8 &name);

	bool RemoveItem(const ehs::UInt_64 &id);

    StackItem *GetItem(const ehs::UInt_64 &id) const;

	StackItem *GetItem(const ehs::Str_8 &name) const;
};
