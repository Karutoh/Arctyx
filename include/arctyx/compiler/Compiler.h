#pragma once

#include <ehs/Array.h>
#include <ehs/Serializer.h>

#include "Architecture.h"
#include "Language.h"
#include "Section.h"
#include "Stack.h"
#include "Symbol.h"

class Compiler
{
private:
	const Architecture *architecture;
	const Language *language;
	ehs::UInt_64 entryPointId;
	ehs::Str_8 entryPointName;
	ehs::Array<Symbol> globalSymbols;
	ehs::Array<Section> sections;

public:
	Compiler();

	Compiler(const ehs::Str_8 &arch, const ehs::Str_8 &lang, ehs::Str_8 entryPoint);

	Compiler(Compiler &&other) noexcept;

	Compiler(const Compiler &other);

	Compiler &operator=(Compiler &&other) noexcept;

	Compiler &operator=(const Compiler &other);

	const Architecture *GetArchitecture() const;

	const Language *GetLanguage() const;

	bool HasSymbol(const SymbolType &type, const ehs::UInt_64 &id) const;

	bool HasSymbol(const SymbolType &type, const ehs::Str_8 &name) const;

	bool AddSymbol(Symbol symbol);

	Symbol *GetSymbol(const SymbolType &type, const ehs::UInt_64 &id) const;

	Symbol *GetSymbol(const SymbolType &type, const ehs::Str_8 &name) const;

	const ehs::Array<Symbol> *GetSymbols() const;

	bool HasSection(const ehs::UInt_64 &id) const;

	bool HasSection(const ehs::Str_8 &name) const;

	bool AddSection(Section section);

	Section *GetSection(const ehs::UInt_64 &id) const;

	Section *GetSection(const ehs::Str_8 &name) const;

	ehs::Vector<ehs::Byte> Compile(const ehs::Str_8 &code);

};