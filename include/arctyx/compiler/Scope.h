#pragma once

#include <ehs/Str.h>
#include <ehs/Vector.h>

enum class ScopeType
{
	UNKNOWN,
	FUNCTION,
	CLASS,
	METHOD,
	NAMESPACE,
	CONSTRUCTOR,
	STRUCTURE
};

class Scope
{
private:
	ehs::UInt_64 id;
	ehs::Str_8 name;
	ScopeType type;
	ehs::Vector<Scope *> children;
	Scope *parent;

public:
	~Scope();

	Scope();

	Scope(ehs::Str_8 name, const ScopeType &type);

	Scope(Scope &&other) noexcept;

	Scope(const Scope &other);

	Scope &operator=(Scope &&other) noexcept;

	Scope &operator=(const Scope &other);

	bool IsValid() const;

	ehs::UInt_64 GetId() const;

	ehs::Str_8 GetName() const;

	ScopeType GetType() const;

	bool HasChild(const ehs::UInt_64 &childId) const;

	bool HasChild(const ehs::Str_8 &childName) const;

	Scope *GetChild(const ehs::UInt_64 &childId) const;

	Scope *GetChild(const ehs::Str_8 &childName) const;

	Scope *AddChild(ehs::Str_8 childName, const ScopeType &childType);

	Scope *GetParent() const;

	ehs::Str_8 ToStr() const;
};