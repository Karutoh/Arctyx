#include "arctyx/compiler/Language.h"

ehs::Array<const Language *> Language::languages;

Language::Language()
	: id(0)
{
}

Language::Language(ehs::Str_8 name, const ehs::Version& version)
	: id(name.Hash_64()), name((ehs::Str_8 &&)name), version(version)
{
}

Language::Language(Language&& lang) noexcept
	: id(lang.id), name((ehs::Str_8 &&)lang.name), version(lang.version), eols((ehs::Array<ehs::Char_8> &&)lang.eols),
	separators((ehs::Array<ehs::Char_8> &&)lang.separators), primitives((ehs::Array<Primitive> &&)lang.primitives),
	keywords((ehs::Array<ehs::Str_8> &&)lang.keywords), operators((ehs::Array<Operator> &&)lang.operators),
	interpretations((ehs::Array<Interpretation> &&)lang.interpretations)
{
	lang.id = 0;
	lang.version = {};
}

Language::Language(const Language& lang)
	: id(lang.id), name(lang.name), version(lang.version), eols(lang.eols), separators(lang.separators),
	primitives(lang.primitives), keywords(lang.keywords), operators(lang.operators),
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
	eols = (ehs::Array<ehs::Char_8> &&)lang.eols;
	separators = (ehs::Array<ehs::Char_8> &&)lang.separators;
	primitives = (ehs::Array<Primitive> &&)lang.primitives;
	keywords = (ehs::Array<ehs::Str_8> &&)lang.keywords;
	operators = (ehs::Array<Operator> &&)lang.operators;
	interpretations = (ehs::Array<Interpretation> &&)lang.interpretations;

	lang.id = 0;
	lang.version = {};

	return *this;
}

Language& Language::operator=(const Language& lang)
{
	if (this == &lang)
		return *this;

	id = lang.id;
	name = lang.name;
	version = lang.version;
	eols = lang.eols;
	separators = lang.separators;
	primitives = lang.primitives;
	keywords = lang.keywords;
	operators = lang.operators;
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

ehs::Array<ehs::Char_8> Language::GetEOLs() const
{
	return eols;
}

bool Language::HasEOL(const ehs::Char_8 &eol) const
{
	for (ehs::UInt_64 i = 0; i < eols.Size(); ++i)
		if (eols[i] == eol)
			return true;

	return false;
}

bool Language::AddEOL(const ehs::Char_8 &eol)
{
	if (HasEOL(eol))
		return false;

	eols.Push(eol);

	return true;
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

ehs::Array<Primitive> Language::GetPrimitives() const
{
	return primitives;
}

bool Language::HasPrimitive(const ehs::UInt_64 &id) const
{
	for (ehs::Size i = 0; i < primitives.Size(); ++i)
		if (primitives[i].GetId() == id)
			return true;

	return false;
}

bool Language::HasPrimitive(const ehs::Str_8 &name) const
{
	return HasPrimitive(name.Hash_64());
}

const Primitive *Language::GetPrimitive(const ehs::UInt_64 &id) const
{
	for (ehs::Size i = 0; i < primitives.Size(); ++i)
		if (primitives[i].GetId() == id)
			return &primitives[i];

	return nullptr;
}

const Primitive* Language::GetPrimitive(const ehs::Str_8 &name) const
{
	return GetPrimitive(name.Hash_64());
}

bool Language::AddPrimitive(Primitive primitive)
{
	if (HasPrimitive(primitive.GetId()))
		return false;

	primitives.Push((Primitive &&)primitive);

	return true;
}

ehs::Array<ehs::Str_8> Language::GetKeywords() const
{
	return keywords;
}

bool Language::HasKeyword(const ehs::Str_8& keyword) const
{
	for (ehs::UInt_64 i = 0; i < keywords.Size(); ++i)
		if (keywords[i] == keyword)
			return true;

	return false;
}

bool Language::AddKeyword(const ehs::Str_8& keyword)
{
	if (HasKeyword(keyword))
		return false;

	keywords.Push((ehs::Str_8 &&)keyword);

	return true;
}

ehs::Array<Operator> Language::GetOperators() const
{
	return operators;
}

bool Language::HasOperator(const ehs::Str_8 &delimeter) const
{
	for (ehs::UInt_64 i = 0; i < operators.Size(); ++i)
		if (operators[i].GetDelimeter() == delimeter)
			return true;

	return false;
}

const Operator* Language::GetOperator(const ehs::Str_8 &delimeter) const
{
	for (ehs::UInt_64 i = 0; i < operators.Size(); ++i)
		if (operators[i].GetDelimeter() == delimeter)
			return &operators[i];

	return nullptr;
}

bool Language::AddOperator(Operator primitive)
{
	if (HasOperator(primitive.GetDelimeter()))
		return false;

	operators.Push((Operator &&)primitive);

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
