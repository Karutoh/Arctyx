#pragma once

#include <ehs/Array.h>
#include <ehs/Serializer.h>

#include "Architecture.h"
#include "Language.h"
#include "Symbol.h"

class Compiler
{
private:
	const Architecture *architecture;
	const Language *language;
	ehs::Array<Symbol> symbols;

public:
	Compiler();

	Compiler(const ehs::Str_8 &arch, const ehs::Str_8 &lang);

	Compiler(Compiler &&other) noexcept;

	Compiler(const Compiler &other);

	Compiler &operator=(Compiler &&other) noexcept;

	Compiler &operator=(const Compiler &other);

	ehs::Array<ehs::Byte> Compile(const ehs::Str_8 &code) const;

private:
	static bool IsSeparator(const ehs::Array<ehs::Char_8> &separators, const ehs::Char_8 *c);

	static bool IsPrimitive(const ehs::Array<Primitive> &primitives, const ehs::Str_8 &value);

	static bool IsKeyword(const ehs::Array<ehs::Str_8> &keywords, const ehs::Str_8 &value);

	static const Operator *IsOperator(const ehs::Array<Operator> &operators, const ehs::Str_8 &value);

	Token ParseValue(const ehs::Array<Primitive> &primitives, const ehs::Array<ehs::Str_8> &keywords, const ehs::Array<Operator> &operators, const ehs::Str_8 &value) const;

	ehs::Vector<Token> Parse(const ehs::Str_8 &code) const;
};