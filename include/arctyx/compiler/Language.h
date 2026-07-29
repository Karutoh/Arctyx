#pragma once

#include <ehs/Array.h>
#include <ehs/Str.h>
#include <ehs/Types.h>
#include <ehs/Version.h>

#include "Interpretation.h"
#include "Operator.h"
#include "Primitive.h"

typedef ehs::Vector<Token> (*CompileIntoTokensCb)(Compiler *compiler, const Language *lang, const ehs::Str_8 &code);
typedef ehs::Vector<ehs::Byte> (*CompileIntoMachineCodeCb)(Compiler *compiler, const Language *lang, const Architecture *arch, const ehs::Vector<Token> &tokens);

class EHS_LIB_IO Language
{
private:
	friend class Arctyx;

	ehs::UInt_64 id;
	ehs::Str_8 name;
	ehs::Version version;
	ehs::Array<ehs::Char_8> eols;
	ehs::Array<Primitive> primitives;
	ehs::Array<ehs::Str_8> keywords;
	ehs::Array<Operator> operators;
	ehs::Array<Interpretation> interpretations;
	CompileIntoTokensCb compileIntoTokensCb;
	CompileIntoMachineCodeCb compileIntoMachineCodeCb;

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

	ehs::Array<ehs::Char_8> GetEOLs() const;

	bool HasEOL(const ehs::Char_8 &eol) const;

	bool AddEOL(const ehs::Char_8 &eol);

	ehs::Array<Primitive> GetPrimitives() const;

	bool HasPrimitive(const ehs::UInt_64 &id) const;

	bool HasPrimitive(const ehs::Str_8 &name) const;

	const Primitive *GetPrimitive(const ehs::UInt_64 &id) const;

	const Primitive *GetPrimitive(const ehs::Str_8 &name) const;

	bool AddPrimitive(Primitive primitive);

	ehs::Array<ehs::Str_8> GetKeywords() const;

	bool HasKeyword(const ehs::Str_8 &keyword) const;

	bool AddKeyword(const ehs::Str_8 &keyword);

	ehs::Array<Operator> GetOperators() const;

	bool HasOperator(const ehs::Str_8 &delimeter) const;

	const Operator *GetOperator(const ehs::Str_8 &delimeter) const;

	bool AddOperator(Operator primitive);

	ehs::Array<Interpretation> GetInterpretations() const;

	bool HasInterpretation(const TokenT &type, const ehs::Str_8 &name) const;

	const Interpretation *GetInterpretation(const TokenT &type, const ehs::Str_8 &name) const;

	bool AddInterpretation(Interpretation interpretation);

	void SetCompileIntoTokensCb(CompileIntoTokensCb cb);

	ehs::Vector<Token> CompileIntoTokens(Compiler *compiler, const ehs::Str_8 &code) const;

	void SetCompileIntoMachineCodeCb(CompileIntoMachineCodeCb cb);

	ehs::Vector<ehs::Byte> CompileIntoMachineCode(Compiler *compiler, const ehs::Vector<Token> &tokens) const;

	static bool Has(const ehs::UInt_64 &id);

	static bool Has(const ehs::Str_8 &name);

	static const Language *Get(const ehs::UInt_64 &id);

	static const Language *Get(const ehs::Str_8 &name);

	static bool Add(Language lang);

};
