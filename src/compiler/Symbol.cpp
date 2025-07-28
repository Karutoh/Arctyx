#include "arctyx/compiler/Symbol.h"

Symbol::Symbol()
	: type(SymbolType::UNKNOWN), id(0), address(0), size(0)
{
}

Symbol::Symbol(const SymbolType& type, ehs::Str_8 name, const ehs::UInt_64& address, const ehs::UInt_64 &size)
	: type(type), id(name.Hash_64()), name((ehs::Str_8 &&)name), address(address), size(size)
{
}

Symbol::Symbol(Symbol&& other) noexcept
	: type(other.type), id(other.id), name((ehs::Str_8 &&)other.name), address(other.address), size(other.size)
{
	other.type = SymbolType::UNKNOWN;
	other.id = 0;
	other.address = 0;
	other.size = 0;
}

Symbol::Symbol(const Symbol &other)
	: type(other.type), id(other.id), name(other.name), address(other.address), size(other.size)
{
}

Symbol &Symbol::operator=(Symbol &&other) noexcept
{
	if (this == &other)
		return *this;

	type = other.type;
	name = (ehs::Str_8 &&)other.name;
	id = other.id;
	address = other.address;
	size = other.size;

	other.type = SymbolType::UNKNOWN;
	other.id = 0;
	other.address = 0;
	other.size = 0;

	return *this;
}

Symbol &Symbol::operator=(const Symbol& other)
{
	if (this == &other)
		return *this;

	type = other.type;
	name = other.name;
	id = other.id;
	address = other.address;
	size = other.size;

	return *this;
}

SymbolType Symbol::GetType() const
{
	return type;
}

ehs::Str_8 Symbol::GetName() const
{
	return name;
}

ehs::UInt_64 Symbol::GetId() const
{
	return id;
}

ehs::UInt_64 Symbol::GetAddress() const
{
	return address;
}

ehs::UInt_64 Symbol::GetSize() const
{
	return size;
}
