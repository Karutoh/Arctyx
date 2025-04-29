#include "arctyx/compiler/Compiler.h"

#include <iterator>
#include <ehs/io/Console.h>

Compiler::Compiler()
    : architecture(nullptr), language(nullptr)
{
}

Compiler::Compiler(const ehs::Str_8& arch, const ehs::Str_8& lang)
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

    EHS_LOG_SUCCESS();
}

Compiler::Compiler(Compiler&& other) noexcept
    : architecture(other.architecture), language(other.language), symbols((ehs::Array<Symbol> &&)other.symbols)
{
    other.architecture = nullptr;
    other.language = nullptr;
}

Compiler::Compiler(const Compiler& other)
    : architecture(other.architecture), language(other.language), symbols(other.symbols)
{
}

Compiler& Compiler::operator=(Compiler&& other) noexcept
{
    if (this == &other)
        return *this;

    architecture = other.architecture;
    language = other.language;
    symbols = (ehs::Array<Symbol> &&)other.symbols;

    other.architecture = nullptr;
    other.language = nullptr;

    return *this;
}

Compiler& Compiler::operator=(const Compiler& other)
{
    if (this == &other)
        return *this;

    architecture = other.architecture;
    language = other.language;
    symbols = other.symbols;

    return *this;
}

ehs::Array<ehs::Byte> Compiler::Compile(const ehs::Str_8 &code) const
{
    ehs::Console::Write_8("Code:");
    ehs::Console::Write_8(code);

    ehs::Console::Write_8("Tokens:");
    ehs::Vector<Token> tokens = Parse(code);
    for (ehs::UInt_64 i = 0; i < tokens.Size(); ++i)
    {
        if (tokens[i].GetType() == TokenT::UNKNOWN)
            ehs::Console::Write_8("UNKNOWN, ", false);
        else if (tokens[i].GetType() == TokenT::SEPARATOR)
            ehs::Console::Write_8("SEPARATOR, ", false);
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
        else if (tokens[i].GetType() == TokenT::EOL)
            ehs::Console::Write_8("EOL");
    }

    ehs::Array<ehs::Byte> machineCode;

    return machineCode;
}

bool Compiler::IsEOL(const ehs::Array<ehs::Char_8>& eols, const ehs::Char_8* c)
{
    for (ehs::UInt_64 s = 0; s < eols.Size(); ++s)
        if (*c == eols[s])
            return true;

    return false;
}

bool Compiler::IsSeparator(const ehs::Array<ehs::Char_8> &separators, const ehs::Char_8 *c)
{
    for (ehs::UInt_64 s = 0; s < separators.Size(); ++s)
        if (*c == separators[s])
            return true;

    return false;
}

bool Compiler::IsPrimitive(const ehs::Array<Primitive>& primitives, const ehs::Str_8& value)
{
    for (ehs::UInt_64 i = 0; i < primitives.Size(); ++i)
        if (value == primitives[i].GetName())
            return true;

    return false;
}

bool Compiler::IsKeyword(const ehs::Array<ehs::Str_8>& keywords, const ehs::Str_8& value)
{
    for (ehs::UInt_64 i = 0; i < keywords.Size(); ++i)
        if (value == keywords[i])
            return true;

    return false;
}

const Operator *Compiler::IsOperator(const ehs::Array<Operator>& operators, const ehs::Str_8& value)
{
    for (ehs::UInt_64 i = 0; i < operators.Size(); ++i)
        if (value == operators[i].GetDelimeter())
            return &operators[i];

    return nullptr;
}

ehs::Vector<Token> Compiler::Parse(const ehs::Str_8 &code) const
{
    ehs::Vector<Token> tokens;

    const ehs::Array<ehs::Char_8> eols = language->GetEOLs();
    const ehs::Array<ehs::Char_8> separators = language->GetSeparators();
    const ehs::Array<Primitive> primitives = language->GetPrimitives();
    const ehs::Array<ehs::Str_8> keywords = language->GetKeywords();
    const ehs::Array<Operator> operators = language->GetOperators();

    for (ehs::Char_8 *i = &code[0], *start = i; i < &code[code.Size()]; ++i)
    {
        if (*start == '\"')
        {
            if (*i == '\"')
            {
                ++start;

                tokens.Push({TokenT::STRING, ehs::Str_8(start, i - 1 - start)});

                if (i + 1 < &code[code.Size()])
                    start = i + 1;
            }
        }
        else if (*start == '\'')
        {
            if (*i == '\'')
            {
                ++start;

                const ehs::UInt_64 size = i - 1 - start;
                if (size > 1)
                {
                    EHS_LOG(ehs::LogType::ERR, 0, "Characters cannot extend to more than one.");

                    return {};
                }

                tokens.Push({TokenT::CHARACTER, {start, size}});

                if (i + 1 < &code[code.Size()])
                    start = i + 1;
            }
        }
        else if (const bool isEOL = IsEOL(eols, i), isSep = IsSeparator(separators, i); *i == ' ' || *i == '\t' || isEOL || isSep)
        {
            if ((*start == ' ' || *start == '\t') && i + 1 < &code[code.Size()])
            {
                start = i + 1;

                continue;
            }

            const ehs::Str_8 value(start, i - start);

            if (value.Size())
            {
                if (IsPrimitive(primitives, value))
                    tokens.Push({TokenT::TYPE, value});
                else if (IsKeyword(keywords, value))
                    tokens.Push({TokenT::KEYWORD, value});
                else if (const Operator *op = IsOperator(operators, value); op)
                {
                    if (op->IsUnary())
                        tokens.Push({TokenT::UNARY_OPERATOR, value});
                    else
                        tokens.Push({TokenT::COMPOUND_OPERATOR, value});
                }
                else if (value[0] >= '0' && value[0] <= '9')
                    tokens.Push({TokenT::NUMBER, value});
                else
                    tokens.Push({TokenT::IDENTIFIER, value});
            }

            if (isEOL)
                tokens.Push({TokenT::EOL, {i, 1}});
            else if (isSep)
                tokens.Push({TokenT::SEPARATOR, {i, 1}});

            start = i + 1;
        }
    }

    EHS_LOG_SUCCESS();

    return tokens;
}
