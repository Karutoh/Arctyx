#pragma once

#include <ehs/Array.h>
#include <ehs/Serializer.h>

#include "Architecture.h"
#include "Language.h"
#include "Stack.h"
#include "Symbol.h"

class Compiler
{
private:
	const Architecture *architecture;
	const Language *language;
	ehs::UInt_64 entryPointId;
	ehs::Str_8 entryPointName;
	ehs::Array<Symbol> symbols;

public:
	Compiler();

	Compiler(const ehs::Str_8 &arch, const ehs::Str_8 &lang, ehs::Str_8 entryPoint);

	Compiler(Compiler &&other) noexcept;

	Compiler(const Compiler &other);

	Compiler &operator=(Compiler &&other) noexcept;

	Compiler &operator=(const Compiler &other);

	bool HasSymbol(const ehs::UInt_64 &id) const;

	bool HasSymbol(const ehs::Str_8 &name) const;

	bool AddSymbol(Symbol symbol);

	Symbol *GetSymbol(const ehs::UInt_64 &id) const;

	Symbol *GetSymbol(const ehs::Str_8 &name) const;

	ehs::Vector<ehs::Byte> Compile(const ehs::Str_8 &code);

private:
	static bool IsNumber(const ehs::Char_8 *c);

	static bool IsOperator(const ehs::Char_8 *c);

	static bool IsAlphabet(const ehs::Char_8 *c);

	static bool IsAlphaNumeric(const ehs::Char_8 *c);

	static bool IsPrimitive(const ehs::Array<Primitive> &primitives, const ehs::Str_8 &value);

	static bool IsKeyword(const ehs::Array<ehs::Str_8> &keywords, const ehs::Str_8 &value);

	static bool IsEncapsulator(const ehs::Char_8 *c);

	static bool IsEOL(const ehs::Array<ehs::Char_8> &eols, const ehs::Char_8 *c);

	static void ParseNumber(ehs::Vector<Token> &tokens, const ehs::Array<ehs::Char_8> &eols, ehs::Char_8 **start, ehs::Char_8 **i);

	static void ParseOperator(ehs::Vector<Token> &tokens, const ehs::Array<ehs::Char_8> &eols, ehs::Char_8 **start, ehs::Char_8 **i);

	static void ParseText(ehs::Vector<Token> &tokens, const ehs::Array<ehs::Char_8> &eols,
		const ehs::Array<Primitive> &primitives, const ehs::Array<ehs::Str_8> &keywords, ehs::Char_8 **start,
		ehs::Char_8 **i);

	ehs::Vector<Token> ParseIntoTokens(const ehs::Str_8 &code) const;
};