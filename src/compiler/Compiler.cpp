#include "arctyx/compiler/Compiler.h"

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
    ehs::Vector<Token> tokens = Parse(code);

    ehs::Array<ehs::Byte> machineCode;

    return machineCode;
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

Token Compiler::ParseValue(const ehs::Array<Primitive> &primitives, const ehs::Array<ehs::Str_8> &keywords, const ehs::Array<Operator> &operators, const ehs::Str_8& value) const
{
    if (IsPrimitive(primitives, value))
        return {TokenT::TYPE, value};

    if (IsKeyword(keywords, value))
        return {TokenT::KEYWORD, value};

    const Operator *op = IsOperator(operators, value);
    if (op)
    {
        if (op->IsUnary())
            return {TokenT::UNARY_OPERATOR, value};
        else
            return {TokenT::COMPOUND_OPERATOR, value};
    }

    if (value[0] >= '0' && value[0] <= '9')
        return {TokenT::VALUE, value};

    return {TokenT::IDENTIFIER, value};
}

ehs::Vector<Token> Compiler::Parse(const ehs::Str_8 &code) const
{
    ehs::Vector<Token> tokens;

    const ehs::Array<ehs::Char_8> separators = language->GetSeparators();
    const ehs::Array<Primitive> primitives = language->GetPrimitives();
    const ehs::Array<ehs::Str_8> keywords = language->GetKeywords();
    const ehs::Array<Operator> operators = language->GetOperators();

    for (ehs::Char_8 *i = &code[0], *start = i; i < &code[code.Size()]; ++i)
    {
        if (*i == language->GetEOL())
        {
            if (start != i)
                tokens.Push(ParseValue(primitives, keywords, operators, ehs::Str_8(start, i - start)));

            tokens.Push({TokenT::EOL, {i, 1}});

            start = i + 1;

            continue;
        }

        if (*start == '\"')
        {
            if (i != start && *i == '\"')
            {
                tokens.Push({TokenT::VALUE, {start, (ehs::UInt_64)(i - (start - 1))}});

                start = i + 1;

                continue;
            }

            continue;
        }

        if (!IsSeparator(separators, i))
            continue;

        if (i - (start - 1) == 1 && IsSeparator(separators, start))
        {
            start = i + 1;
            continue;
        }

        tokens.Push(ParseValue(primitives, keywords, operators, ehs::Str_8(start, i - start)));

        start = i + 1;
    }

    return tokens;
}
