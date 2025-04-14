#pragma once

#include <ehs/Str.h>

enum class TokenType : ehs::UInt_8
{
	VALUE,
	KEYWORD,
	IDENTIFIER,
	UNARY_OPERATOR,
	COMPOUND_OPERATOR
};

class Token
{
private:
	TokenType type;
	ehs::Str_8 value;

public:
	Token();

	Token(TokenType type, ehs::Str_8 value);

	Token(Token &&token) noexcept;

	Token(const Token &token);

	Token &operator=(Token &&token) noexcept;

	Token &operator=(const Token &token);

	TokenType GetType() const;

	ehs::Str_8 GetValue() const;
};
