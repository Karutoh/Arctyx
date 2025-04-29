#pragma once

#include <ehs/Str.h>

enum class TokenT : ehs::UInt_8
{
	UNKNOWN,
	SEPARATOR,
	NUMBER,
	STRING,
	CHARACTER,
	KEYWORD,
	TYPE,
	IDENTIFIER,
	UNARY_OPERATOR,
	COMPOUND_OPERATOR,
	EOL
};

class EHS_LIB_IO Token
{
private:
	TokenT type;
	ehs::Str_8 value;

public:
	Token();

	Token(const TokenT &type, ehs::Str_8 value);

	Token(Token &&token) noexcept;

	Token(const Token &token);

	Token &operator=(Token &&token) noexcept;

	Token &operator=(const Token &token);

	TokenT GetType() const;

	ehs::Str_8 GetValue() const;
};
