#include "arctyx/compiler/Scope.h"

Scope::~Scope()
{
	if (parent)
	{
		for (ehs::UInt_64 i = 0; i < parent->children.Size(); ++i)
		{
			if (parent->children[i]->id != id)
				continue;

			parent->children.Swap(i, parent->children.End());
			parent->children.Pop();

			break;
		}
	}

	for (ehs::UInt_64 i = 0; i < children.Size(); ++i)
		delete children[i];
}

Scope::Scope()
	: id(0), type(ScopeType::UNKNOWN), parent(nullptr)
{
}

Scope::Scope(ehs::Str_8 name, const ScopeType &type)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), type(type), parent(nullptr)
{
}

Scope::Scope(Scope&& other) noexcept
	: id(other.id), name((ehs::Str_8 &&)other.name), type(other.type), children((ehs::Vector<Scope *> &&)other.children),
	parent(other.parent)
{
	for (ehs::UInt_64 i = 0; i < children.Size(); ++i)
		children[i]->parent = this;

	for (ehs::UInt_64 i = 0; i < parent->children.Size(); ++i)
		if (parent->children[i]->id == id)
			parent->children[i] = this;

	other.id = 0;
	other.type = ScopeType::UNKNOWN;
	other.parent = nullptr;
}

Scope::Scope(const Scope& other)
	: id(other.id), name(other.name), type(other.type), parent(nullptr)
{
}

Scope& Scope::operator=(Scope&& other) noexcept
{
	if (this == &other)
		return *this;

	id = other.id;
	name = (ehs::Str_8 &&)other.name;
	type = other.type;

	children = (ehs::Vector<Scope *> &&)other.children;
	for (ehs::UInt_64 i = 0; i < children.Size(); ++i)
		children[i]->parent = this;

	parent = other.parent;
	if (parent)
		for (ehs::UInt_64 i = 0; i < parent->children.Size(); ++i)
			if (parent->children[i]->id == id)
				parent->children[i] = this;

	other.id = 0;
	other.type = ScopeType::UNKNOWN;
	other.parent = nullptr;

	return *this;
}

Scope& Scope::operator=(const Scope& other)
{
	if (this == &other)
		return *this;

	id = other.id;
	name = other.name;
	type = other.type;
	children = {};
	parent = nullptr;

	return *this;
}

bool Scope::IsValid() const
{
	return id;
}

ehs::UInt_64 Scope::GetId() const
{
	return id;
}

ehs::Str_8 Scope::GetName() const
{
	return name;
}

ScopeType Scope::GetType() const
{
	return type;
}

bool Scope::HasChild(const ehs::UInt_64& childId) const
{
	for (ehs::UInt_64 i = 0; i < children.Size(); ++i)
		if (children[i]->id == childId)
			return true;

	return false;
}

bool Scope::HasChild(const ehs::Str_8& childName) const
{
	return HasChild(childName.Hash_64());
}

Scope *Scope::GetChild(const ehs::UInt_64& childId) const
{
	for (ehs::UInt_64 i = 0; i < children.Size(); ++i)
		if (children[i]->id == childId)
			return children[i];

	return nullptr;
}

Scope* Scope::GetChild(const ehs::Str_8& childName) const
{
	return GetChild(childName.Hash_64());
}

Scope *Scope::AddChild(ehs::Str_8 childName, const ScopeType &childType)
{
	const ehs::UInt_64 childId = childName.Hash_64();

	if (HasChild(childId))
		return nullptr;

	Scope *child = new Scope();
	child->id = childId;
	child->name = (ehs::Str_8 &&)childName;
	child->type = childType;
	child->parent = this;
	children.Push(child);

	return child;
}

Scope* Scope::GetParent() const
{
	return parent;
}

ehs::Str_8 Scope::ToStr() const
{
	ehs::Str_8 result;
	const Scope *next = this;
	while (next)
	{
		if (next->GetType() == ScopeType::NAMESPACE)
			result = next->name + "::" + result;
		else
			result = next->name + "." + result;

		next = next->parent;
	}

	return result;
}
