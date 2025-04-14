#include "arctyx/compiler/Token.h"

Token::Token()
{
}

Token::Token(TokenType type, ehs::Str_8 value)
{
}

Token::Token(Token &&token) noexcept
{
}

Token::Token(const Token &token)
{
}

Token & Token::operator=(Token &&token) noexcept
{
	return *this;
}

Token & Token::operator=(const Token &token)
{
	return *this;
}

TokenType Token::GetType() const
{
	return type;
}

ehs::Str_8 Token::GetValue() const
{
	return {};
}
