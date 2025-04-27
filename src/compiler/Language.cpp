#include "arctyx/compiler/Language.h"

ehs::Array<const Language *> Language::languages;

Language::Language()
	: id(0), eol('\n')
{
}

Language::Language(ehs::Str_8 name, const ehs::Version& version)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), version(version), eol('\n')
{
}

Language::Language(Language&& lang) noexcept
	: id(lang.id), name((ehs::Str_8 &&)lang.name), version(lang.version), eol(lang.eol),
	separators((ehs::Array<ehs::Char_8> &&)lang.separators),
	interpretations((ehs::Array<Interpretation> &&)lang.interpretations)
{
	lang.id = 0;
	lang.version = {};
	lang.eol = '\n';
}

Language::Language(const Language& lang)
	: id(lang.id), name(lang.name), version(lang.version), eol(lang.eol), separators(lang.separators),
	interpretations(lang.interpretations)
{
}

Language& Language::operator=(Language&& lang) noexcept
{
	if (this == &lang)
		return *this;

	id = lang.id;
	name = (ehs::Str_8 &&)lang.name;
	version = lang.version;
	eol = lang.eol;
	separators = (ehs::Array<ehs::Char_8> &&)lang.separators;
	interpretations = (ehs::Array<Interpretation> &&)lang.interpretations;

	lang.id = 0;
	lang.version = {};
	lang.eol = '\n';

	return *this;
}

Language& Language::operator=(const Language& lang)
{
	if (this == &lang)
		return *this;

	id = lang.id;
	name = lang.name;
	version = lang.version;
	eol = lang.eol;
	separators = lang.separators;
	interpretations = lang.interpretations;

	return *this;
}

ehs::UInt_64 Language::GetId() const
{
	return id;
}

ehs::Str_8 Language::GetName() const
{
	return name;
}

ehs::Version Language::GetVersion() const
{
	return version;
}

void Language::SetEOL(const ehs::Char_8& newEOL)
{
	eol = newEOL;
}

ehs::Char_8 Language::GetEOL() const
{
	return eol;
}

ehs::Array<ehs::Char_8> Language::GetSeparators() const
{
	return separators;
}

bool Language::HasSeparator(const ehs::Char_8& separator) const
{
	for (ehs::Size i = 0; i < separators.Size(); ++i)
		if (separators[i] == separator)
			return true;

	return false;
}

bool Language::AddSeparator(const ehs::Char_8& separator)
{
	if (HasSeparator(separator))
		return false;

	separators.Push(separator);

	return true;
}

ehs::Array<Interpretation> Language::GetInterpretations() const
{
	return interpretations;
}

bool Language::HasInterpretation(const TokenT &type, const ehs::Str_8& name) const
{
	for (ehs::Size i = 0; i < interpretations.Size(); ++i)
		if (interpretations[i].GetType() == type && interpretations[i].GetValue() == name)
			return true;

	return false;
}

const Interpretation *Language::GetInterpretation(const TokenT &type, const ehs::Str_8& name) const
{
	for (ehs::Size i = 0; i < interpretations.Size(); ++i)
		if (interpretations[i].GetType() == type && interpretations[i].GetValue() == name)
			return &interpretations[i];

	return nullptr;
}

bool Language::AddInterpretation(Interpretation interpretation)
{
	if (HasInterpretation(interpretation.GetType(), interpretation.GetValue()))
		return false;

	interpretations.Push((Interpretation &&)interpretation);

	return true;
}

bool Language::Has(const ehs::UInt_64& id)
{
	for (ehs::UInt_64 i = 0; i < languages.Size(); ++i)
		if (languages[i]->GetId() == id)
			return true;

	return false;
}

bool Language::Has(const ehs::Str_8& name)
{
	return Has(name.Hash_64());
}

const Language* Language::Get(const ehs::UInt_64& id)
{
	for (ehs::UInt_64 i = 0; i < languages.Size(); ++i)
		if (languages[i]->GetId() == id)
			return languages[i];

	return nullptr;
}

const Language* Language::Get(const ehs::Str_8& name)
{
	return Get(name.Hash_64());
}

bool Language::Add(Language lang)
{
	if (Has(lang.GetId()))
		return false;

	languages.Push(new Language((Language &&)lang));

	return true;
}
