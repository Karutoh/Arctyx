#pragma once
#include <ehs/Str.h>

#include "Instruction.h"

class EHS_LIB_IO Keyword
{
private:
	ehs::Str_8 identifier;
	Instruction instruction;
};
