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

	Compiler(const Architecture *arch, const Language *lang);
};