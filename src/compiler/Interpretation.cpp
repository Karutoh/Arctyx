#include "arctyx/compiler/Interpretation.h"

Interpretation::Interpretation(const TokenT& type, ehs::Str_8 value, Instruction result)
	: Token(type, (ehs::Str_8 &&)value), result((Instruction &&)result)
{
}

Interpretation::Interpretation(Interpretation&& other) noexcept
	: Token((Token &&)other), result((Instruction &&)other.result)
{
}

Interpretation::Interpretation(const Interpretation& other)
	: Token(other), result(other.result)
{
}

Interpretation& Interpretation::operator=(Interpretation&& other) noexcept
{
	if (this == &other)
		return *this;

	Token::operator=((Token &&)other);

	result = (Instruction &&)other.result;

	return *this;
}

Interpretation& Interpretation::operator=(const Interpretation& other)
{
	if (this == &other)
		return *this;

	Token::operator=(other);

	result = other.result;

	return *this;
}

Instruction Interpretation::GetResult() const
{
	return result;
}
