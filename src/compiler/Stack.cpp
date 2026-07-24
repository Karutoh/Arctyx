#include "arctyx/compiler/Stack.h"

Stack::Stack()
	: offset(0)
{
}

Stack::Stack(const ehs::UInt_64 &offset)
	: offset(offset)
{
}

Stack::Stack(Stack &&other) noexcept
	: inputs((ehs::Array<StackParam> &&)other.inputs), outputs((ehs::Array<StackParam> &&)other.outputs),
	offset(other.offset), items((ehs::Vector<StackItem>&&)other.items)
{
}

Stack::Stack(const Stack &other)
	: inputs(other.inputs), outputs(other.outputs), offset(other.offset), items(other.items)
{
}

Stack& Stack::operator=(Stack &&other) noexcept
{
	if (this == &other)
		return *this;

	inputs = (ehs::Array<StackParam> &&)other.inputs;
	outputs = (ehs::Array<StackParam> &&)other.outputs;
	offset = other.offset;
	items = (ehs::Vector<StackItem> &&)other.items;

	other.offset = 0;

	return *this;
}

Stack& Stack::operator=(const Stack &other)
{
	if (this == &other)
		return *this;

	inputs = other.inputs;
	outputs = other.outputs;
	offset = other.offset;
	items = other.items;

	return *this;
}

ehs::UInt_64 Stack::GetOffset() const
{
	return offset;
}

bool Stack::HasItem(const ehs::UInt_64& id) const
{
	for (ehs::UInt_64 i = 0; i < items.Size(); ++i)
		if (items[i].GetId() == id)
			return true;

	return false;
}

bool Stack::HasItem(const ehs::Str_8& name) const
{
	return HasItem(name.Hash_64());
}

void Stack::AddItem(StackItem item)
{
	items.Push((StackItem &&)item);
}

bool Stack::RemoveItem(const ehs::UInt_64 &id)
{
	for (ehs::UInt_64 i = 0; i < items.Size(); ++i)
	{
		if (items[i].GetId() != id)
			continue;

		items.Remove(id);

		return true;
	}

	return false;
}

bool Stack::RemoveItem(const ehs::Str_8& name)
{
	return RemoveItem(name.Hash_64());
}

StackItem* Stack::GetItem(const ehs::UInt_64 &id) const
{
	for (ehs::UInt_64 i = 0; i < items.Size(); ++i)
		if (items[i].GetId() == id)
			return &items[i];

	return nullptr;
}

StackItem* Stack::GetItem(const ehs::Str_8& name) const
{
	return GetItem(name.Hash_64());
}