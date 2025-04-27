#pragma once

#include <ehs/Array.h>
#include <ehs/Str.h>
#include <ehs/Types.h>
#include <ehs/Version.h>

#include "Interpretation.h"

class EHS_LIB_IO Language
{
private:
	friend class Arctyx;

	ehs::UInt_64 id;
	ehs::Str_8 name;
	ehs::Version version;
	ehs::Char_8 eol;
	ehs::Array<ehs::Char_8> separators;
	ehs::Array<Interpretation> interpretations;

	static ehs::Array<const Language *> languages;

public:
	Language();

	Language(ehs::Str_8 name, const ehs::Version &version);

	Language(Language &&lang) noexcept;

	Language(const Language &lang);

	Language &operator=(Language &&lang) noexcept;

	Language &operator=(const Language &lang);

	ehs::UInt_64 GetId() const;

	ehs::Str_8 GetName() const;

	ehs::Version GetVersion() const;

	void SetEOL(const ehs::Char_8 &newEOL);

	ehs::Char_8 GetEOL() const;

	ehs::Array<ehs::Char_8> GetSeparators() const;

	bool HasSeparator(const ehs::Char_8 &separator) const;

	bool AddSeparator(const ehs::Char_8 &separator);

	ehs::Array<Interpretation> GetInterpretations() const;

	bool HasInterpretation(const TokenT &type, const ehs::Str_8 &name) const;

	const Interpretation *GetInterpretation(const TokenT &type, const ehs::Str_8 &name) const;

	bool AddInterpretation(Interpretation interpretation);

	static bool Has(const ehs::UInt_64 &id);

	static bool Has(const ehs::Str_8 &name);

	static const Language *Get(const ehs::UInt_64 &id);

	static const Language *Get(const ehs::Str_8 &name);

	static bool Add(Language lang);

};
