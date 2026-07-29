#include "arctyx/compiler/Compiler.h"

#include <ehs/io/Console.h>

Compiler::Compiler()
    : architecture(nullptr), language(nullptr), entryPointId(0)
{
}

Compiler::Compiler(const ehs::Str_8& arch, const ehs::Str_8& lang, ehs::Str_8 entryPoint)
{
    architecture = Architecture::Get(arch);
    if (!architecture)
    {
        EHS_LOG(ehs::LogType::ERR, 0, "The Architecture, \"" + arch + "\" is not supported.");
        return;
    }

    language = Language::Get(lang);
    if (!language)
    {
        EHS_LOG(ehs::LogType::ERR, 0, "The Language, \"" + lang + "\" is not supported.");
        return;
    }

    entryPointId = entryPoint.Hash_64();
    entryPointName = (ehs::Str_8 &&)entryPoint;

    EHS_LOG_SUCCESS();
}

Compiler::Compiler(Compiler&& other) noexcept
    : architecture(other.architecture), language(other.language), entryPointId(other.entryPointId),
    entryPointName((ehs::Str_8 &&)other.entryPointName), globalSymbols((ehs::Array<Symbol> &&)other.globalSymbols),
	sections((ehs::Array<Section> &&)other.sections)
{
    other.architecture = nullptr;
    other.language = nullptr;
    other.entryPointId = 0;
}

Compiler::Compiler(const Compiler& other)
    : architecture(other.architecture), language(other.language), entryPointId(other.entryPointId),
    entryPointName(other.entryPointName), globalSymbols(other.globalSymbols), sections(other.sections)
{
}

Compiler& Compiler::operator=(Compiler&& other) noexcept
{
    if (this == &other)
        return *this;

    architecture = other.architecture;
    language = other.language;
    entryPointId = other.entryPointId;
    entryPointName = (ehs::Str_8 &&)other.entryPointName;
    globalSymbols = (ehs::Array<Symbol> &&)other.globalSymbols;
	sections = (ehs::Array<Section> &&)other.sections;

    other.architecture = nullptr;
    other.language = nullptr;
    other.entryPointId = 0;

    return *this;
}

Compiler& Compiler::operator=(const Compiler& other)
{
    if (this == &other)
        return *this;

    architecture = other.architecture;
    language = other.language;
    entryPointId = other.entryPointId;
    entryPointName = other.entryPointName;
    globalSymbols = other.globalSymbols;
	sections = other.sections;

    return *this;
}

const Architecture* Compiler::GetArchitecture() const
{
	return architecture;
}

const Language* Compiler::GetLanguage() const
{
	return language;
}

bool Compiler::HasSymbol(const SymbolType &type, const ehs::UInt_64 &id) const
{
    for (ehs::UInt_64 i = 0; i < globalSymbols.Size(); ++i)
        if (globalSymbols[i].GetType() == type && globalSymbols[i].GetId() == id)
            return true;

    return false;
}

bool Compiler::HasSymbol(const SymbolType &type, const ehs::Str_8 &name) const
{
    return HasSymbol(type, name.Hash_64());
}

bool Compiler::AddSymbol(Symbol symbol)
{
    if (HasSymbol(symbol.GetType(), symbol.GetId()))
        return false;

    globalSymbols.Push((Symbol &&)symbol);

    return true;
}

Symbol *Compiler::GetSymbol(const SymbolType &type, const ehs::UInt_64 &id) const
{
    for (ehs::UInt_64 i = 0; i < globalSymbols.Size(); ++i)
        if (globalSymbols[i].GetType() == type && globalSymbols[i].GetId() == id)
            return &globalSymbols[i];

    return nullptr;
}

Symbol *Compiler::GetSymbol(const SymbolType &type, const ehs::Str_8 &name) const
{
    return GetSymbol(type, name.Hash_64());
}

const ehs::Array<Symbol>* Compiler::GetSymbols() const
{
	return &globalSymbols;
}

bool Compiler::HasSection(const ehs::UInt_64& id) const
{
	for (ehs::UInt_64 i = 0; i < sections.Size(); ++i)
		if (sections[i].GetId() == id)
			return true;

	return false;
}

bool Compiler::HasSection(const ehs::Str_8& name) const
{
	return HasSection(name.Hash_64());
}

bool Compiler::AddSection(Section section)
{
	if (HasSection(section.GetId()))
		return false;

	sections.Push((Section &&)section);

	return true;
}

Section* Compiler::GetSection(const ehs::UInt_64& id) const
{
	for (ehs::UInt_64 i = 0; i < sections.Size(); ++i)
		if (sections[i].GetId() == id)
			return &sections[i];

	return nullptr;
}

Section* Compiler::GetSection(const ehs::Str_8& name) const
{
	return GetSection(name.Hash_64());
}

ehs::Vector<ehs::Byte> Compiler::Compile(const ehs::Str_8 &code)
{
	sections.Clear();

    ehs::Console::Write_8("Code:");
    ehs::Console::Write_8(code);

    ehs::Vector<Token> tokens = language->CompileIntoTokens(this, code);

	ehs::Console::Write_8("\nTokens:");
    for (ehs::UInt_64 i = 0; i < tokens.Size(); ++i)
    {
        if (tokens[i].GetType() == TokenT::UNKNOWN)
            ehs::Console::Write_8("UNKNOWN, ", false);
        else if (tokens[i].GetType() == TokenT::SEPARATOR)
            ehs::Console::Write_8("SEPARATOR, ", false);
        else if (tokens[i].GetType() == TokenT::BOOLEAN)
            ehs::Console::Write_8("BOOLEAN, ", false);
        else if (tokens[i].GetType() == TokenT::NUMBER)
            ehs::Console::Write_8("NUMBER, ", false);
        else if (tokens[i].GetType() == TokenT::STRING)
            ehs::Console::Write_8("STRING, ", false);
        else if (tokens[i].GetType() == TokenT::CHARACTER)
            ehs::Console::Write_8("CHARACTER, ", false);
        else if (tokens[i].GetType() == TokenT::KEYWORD)
            ehs::Console::Write_8("KEYWORD, ", false);
        else if (tokens[i].GetType() == TokenT::TYPE)
            ehs::Console::Write_8("TYPE, ", false);
        else if (tokens[i].GetType() == TokenT::IDENTIFIER)
            ehs::Console::Write_8("IDENTIFIER, ", false);
        else if (tokens[i].GetType() == TokenT::UNARY_OPERATOR)
            ehs::Console::Write_8("UNARY_OPERATOR, ", false);
        else if (tokens[i].GetType() == TokenT::COMPOUND_OPERATOR)
            ehs::Console::Write_8("COMPOUND_OPERATOR, ", false);
        else if (tokens[i].GetType() == TokenT::ENCAPSULATOR)
            ehs::Console::Write_8("ENCAPSULATOR, ", false);
        else if (tokens[i].GetType() == TokenT::EOL)
            ehs::Console::Write_8("EOL");
    }

    ehs::Vector<ehs::Byte> machineCode = language->CompileIntoMachineCode(this, tokens);

    ehs::Console::Write_8("\n\nSymbols:");
    for (ehs::UInt_64 i = 0; i < globalSymbols.Size(); ++i)
    {
    	ehs::Console::Write_8("Type: ", false);

		if (globalSymbols[i].GetType() == SymbolType::VARIABLE)
            ehs::Console::Write_8("Variable", false);
		else if (globalSymbols[i].GetType() == SymbolType::FUNCTION)
            ehs::Console::Write_8("Function", false);
		else if (globalSymbols[i].GetType() == SymbolType::CLASS)
            ehs::Console::Write_8("Class", false);
		else if (globalSymbols[i].GetType() == SymbolType::MEMBER)
            ehs::Console::Write_8("Member", false);
		else if (globalSymbols[i].GetType() == SymbolType::METHOD)
            ehs::Console::Write_8("Method", false);
		else if (globalSymbols[i].GetType() == SymbolType::CONSTRUCTOR)
    		ehs::Console::Write_8("Constructor", false);
		else if (globalSymbols[i].GetType() == SymbolType::STRUCTURE)
    		ehs::Console::Write_8("Structure", false);
		else
            ehs::Console::Write_8("Unknown ", false);

        ehs::Console::Write_8(", Name: " + globalSymbols[i].GetName() + ", Id: " + globalSymbols[i].GetId() + ", Line: " + globalSymbols[i].GetLine());
    }

    Symbol *entryPoint = GetSymbol(SymbolType::FUNCTION, entryPointId);
    if (!entryPoint)
        ehs::Console::Write_8("\nCompiler Error: Function symbol, \"" + entryPointName + "\", for the entry point was not found!");

    return machineCode;
}
