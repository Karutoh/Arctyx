#pragma once

#include <ehs/Str.h>
#include <ehs/Types.h>

enum class SymbolType
{
	UNKNOWN,
	VARIABLE,
	FUNCTION,
	CLASS,
	MEMBER,
	METHOD,
	CONSTRUCTOR,
	STRUCTURE
};

class Symbol
{
private:
	SymbolType type;
	ehs::UInt_64 id;
	ehs::Str_8 name;
	ehs::UInt_64 line;
	ehs::UInt_64 address;
	ehs::UInt_64 size;

public:
	Symbol();

	Symbol(const SymbolType &type, ehs::Str_8 name, const ehs::UInt_64 &line);

	Symbol(Symbol &&other) noexcept;

	Symbol(const Symbol &other);

	Symbol &operator=(Symbol &&other) noexcept;

	Symbol &operator=(const Symbol &other);

	SymbolType GetType() const;

	ehs::Str_8 GetName() const;

	ehs::UInt_64 GetId() const;

	ehs::UInt_64 GetLine() const;

	void SetAddress(const ehs::UInt_64 &newAddress);

	ehs::UInt_64 GetAddress() const;

	void SetSize(const ehs::UInt_64 &newSize);

	ehs::UInt_64 GetSize() const;
};