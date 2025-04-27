#pragma once

#include <ehs/Array.h>

#include "Instruction.h"
#include "Token.h"

class EHS_LIB_IO Interpretation : public Token
{
private:
	Instruction result;

public:
	Interpretation() = default;

	Interpretation(const TokenT &type, ehs::Str_8 value, Instruction result);

	Interpretation(Interpretation &&other) noexcept;

	Interpretation(const Interpretation &other);

	Interpretation &operator=(Interpretation &&other) noexcept;

	Interpretation &operator=(const Interpretation &other);

	Instruction GetResult() const;
};
