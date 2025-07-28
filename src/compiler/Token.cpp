#include "arctyx/compiler/Token.h"

Token::Token()
	: type(TokenT::UNKNOWN)
{
}

Token::Token(const TokenT &type, ehs::Str_8 value)
	: type(type), value((ehs::Str_8 &&)value)
{
}

Token::Token(Token &&token) noexcept
	: type(token.type), value((ehs::Str_8 &&)token.value)
{
	token.type = TokenT::UNKNOWN;
}

Token::Token(const Token &token)
	: type(token.type), value(token.value)
{
}

Token & Token::operator=(Token &&token) noexcept
{
	if (this == &token)
		return *this;

	type = token.type;
	value = (ehs::Str_8 &&)token.value;

	token.type = TokenT::UNKNOWN;

	return *this;
}

Token & Token::operator=(const Token &token)
{
	if (this == &token)
		return *this;

	type = token.type;
	value = token.value;

	return *this;
}

void Token::SetType(const TokenT& newType)
{
	type = newType;
}

TokenT Token::GetType() const
{
	return type;
}

ehs::Str_8 Token::GetValue() const
{
	return value;
}
