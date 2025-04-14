#pragma once

#include <ehs/Array.h>
#include <ehs/Str.h>
#include <ehs/Types.h>

#include "Combination.h"

class Language
{
private:
	ehs::UInt_64 id;
	ehs::Str_8 name;
	ehs::Array<ehs::Str_8> keywords;
	ehs::Array<ehs::Char_8> separators;
	ehs::Array<Combination> interpretations;

public:
	Language();

	Language(ehs::Str_8 name);

	Language(Language &&lang) noexcept;

	Language(const Language &lang);

	Language &operator=(Language &&lang) noexcept;

	Language &operator=(const Language &lang);

	ehs::UInt_64 GetId() const;

	ehs::Str_8 GetName() const;

};
