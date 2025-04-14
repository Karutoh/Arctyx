#pragma once
#include <ehs/Str.h>

#include "Instruction.h"

class Keyword
{
private:
	ehs::Str_8 identifier;
	Instruction instruction;
};
