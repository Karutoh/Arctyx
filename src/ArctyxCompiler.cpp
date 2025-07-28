#include "arctyxLang/ArctyxCompiler.h"

bool IsNumber(const ehs::Char_8* c)
{
    if (*c >= '0' && *c <= '9')
        return true;

    return false;
}

bool IsOperator(const ehs::Char_8* c)
{
    if (*c == '!' || *c == ':' || *c == '\\' || *c == '|' || *c == '^' || *c == '`' || *c == '~' || *c == '*'
        || *c == '+' || (*c >= '-' && *c <= '/') || (*c >= '#' && *c <= '&') || (*c >= '<' && *c <= '@'))
        return true;

    return false;
}

bool IsAlphabet(const ehs::Char_8* c)
{
    if ((*c >= 'A' && *c <= 'Z') || (*c >= 'a' && *c <= 'z') || *c == '_')
        return true;

    return false;
}

bool IsAlphaNumeric(const ehs::Char_8 *c)
{
    if ((*c >= 'A' && *c <= 'Z') || (*c >= 'a' && *c <= 'z') || *c == '_' || (*c >= '0' && *c <= '9'))
        return true;

    return false;
}

bool IsPrimitive(const ehs::Array<Primitive>& primitives, const ehs::Str_8& value)
{
    for (ehs::UInt_64 i = 0; i < primitives.Size(); ++i)
        if (value == primitives[i].GetName())
            return true;

    return false;
}

bool IsKeyword(const ehs::Array<ehs::Str_8>& keywords, const ehs::Str_8& value)
{
    for (ehs::UInt_64 i = 0; i < keywords.Size(); ++i)
        if (value == keywords[i])
            return true;

    return false;
}

bool IsEncapsulator(const ehs::Char_8* c)
{
    switch (*c)
    {
    case '(':
        return true;
    case '{':
        return true;
    case '[':
        return true;
    case ')':
        return true;
    case '}':
        return true;
    case ']':
        return true;
    default:
        return false;
    }
}

bool IsEOL(const ehs::Array<ehs::Char_8>& eols, const ehs::Char_8* c)
{
    for (ehs::UInt_64 s = 0; s < eols.Size(); ++s)
        if (*c == eols[s])
            return true;

    return false;
}

void ParseNumber(ehs::Vector<Token>& tokens, const ehs::Array<ehs::Char_8> &eols, ehs::Char_8 **start, ehs::Char_8 **i)
{
    if (const bool isEOL = IsEOL(eols, *i), isSpace = **i == ' ', isEnd = *i != *start && !IsNumber(*i); isEOL || isSpace || isEnd)
    {
        tokens.Push({TokenT::NUMBER, ehs::Str_8(*start, *i - *start)});

        if (isEOL)
            tokens.Push({TokenT::EOL, {*i, 1}});
        else if (!isSpace && isEnd)
        {
            *start = *i;

            return;
        }

        *start = *i + 1;
    }

    ++*i;
}

void ParseOperator(ehs::Vector<Token>& tokens, const ehs::Array<ehs::Char_8> &eols, ehs::Char_8 **start, ehs::Char_8 **i)
{
    if (const bool isEOL = IsEOL(eols, *i), isSpace = **i == ' ', isEnd = *i != *start && !IsOperator(*i); isEOL || isSpace || isEnd)
    {
        if (*i - *start > 1)
            tokens.Push({TokenT::COMPOUND_OPERATOR, ehs::Str_8(*start, *i - *start)});
        else
            tokens.Push({TokenT::UNARY_OPERATOR, ehs::Str_8(*start, *i - *start)});

        if (isEOL)
            tokens.Push({TokenT::EOL, {*i, 1}});
        else if (!isSpace && isEnd)
        {
            *start = *i;

            return;
        }

        *start = *i + 1;
    }

    ++*i;
}

void ParseText(ehs::Vector<Token>& tokens, const ehs::Array<ehs::Char_8>& eols,
    const ehs::Array<Primitive> &primitives, const ehs::Array<ehs::Str_8> &keywords, ehs::Char_8** start,
    ehs::Char_8** i)
{
    if (const bool isEOL = IsEOL(eols, *i), isSpace = **i == ' ', isEnd = *i != *start && !IsAlphaNumeric(*i); isEOL || isSpace || isEnd)
    {
        ehs::Str_8 text(*start, *i - *start);

        if (IsPrimitive(primitives, text))
            tokens.Push({TokenT::TYPE, text});
        else if (IsKeyword(keywords, text))
            tokens.Push({TokenT::KEYWORD, text});
        else if (text == "true" || text == "false")
            tokens.Push({TokenT::BOOLEAN, text});
        else
        {
            if (tokens[tokens.End()].GetType() == TokenT::IDENTIFIER)
                tokens[tokens.End()].SetType(TokenT::TYPE);

            tokens.Push({TokenT::IDENTIFIER, text});
        }

        if (isEOL)
            tokens.Push({TokenT::EOL, {*i, 1}});
        else if (!isSpace && isEnd)
        {
            *start = *i;

            return;
        }

        *start = *i + 1;
    }

    ++*i;
}

ehs::Vector<Token> CompileIntoTokens(Compiler *compiler, const Language *lang, const ehs::Str_8 &code)
{
    ehs::Vector<Token> tokens;

    const ehs::Array<ehs::Char_8> eols = lang->GetEOLs();
    const ehs::Array<Primitive> primitives = lang->GetPrimitives();
    const ehs::Array<ehs::Str_8> keywords = lang->GetKeywords();
    const ehs::Array<Operator> operators = lang->GetOperators();

    for (ehs::Char_8 *i = &code[0], *start = i; i < &code[code.Size()];)
    {
        if (*start == '\"')
        {
            if (start != i && *i == '\"')
            {
                ++start;

                tokens.Push({TokenT::STRING, ehs::Str_8(start, i - start)});

                if (i + 1 < &code[code.Size()])
                    start = i + 1;
            }

            ++i;
        }
        else if (*start == '\'')
        {
            if (start != i && *i == '\'')
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

            ++i;
        }
        else if (IsNumber(start))
            ParseNumber(tokens, eols, &start, &i);
        else if (IsEncapsulator(start))
        {
            tokens.Push({TokenT::ENCAPSULATOR, {i++, 1}});
            start = i;
        }
        else if (IsOperator(start))
            ParseOperator(tokens, eols, &start, &i);
        else if (IsAlphabet(start))
            ParseText(tokens, eols, primitives, keywords, &start, &i);
        else if (*start == ',')
        {
            tokens.Push({TokenT::SEPARATOR, {i++, 1}});
            start = i;
        }
        else if (IsEOL(eols, start))
        {
            if (tokens[tokens.End()].GetType() != TokenT::EOL)
                tokens.Push({TokenT::EOL, {i, 1}});

            start = ++i;
        }
        else
            start = ++i;
    }

    EHS_LOG_SUCCESS();

    return tokens;
}

ehs::Vector<ehs::Array<Token>> SplitLines(const ehs::Vector<Token>& tokens)
{
    ehs::Vector<ehs::Array<Token>> result;

    for (ehs::UInt_64 i = 0, start = 0; i < tokens.Size(); ++i)
    {
        if (tokens[i].GetType() != TokenT::EOL)
            continue;

        result.Push(ehs::Array<Token>(&tokens[start], i - start));

        start = i + 1;
    }

    return result;
}

bool IsFunction(const ehs::Array<Token>& line)
{
    for (ehs::UInt_64 i = 0; i < line.Size(); ++i)
        if ((line[i].GetType() == TokenT::SEPARATOR && line[i].GetValue() == ",") ||
            (i + 1 < line.Size() && line[i].GetType() == TokenT::TYPE && line[i + 1].GetType() == TokenT::IDENTIFIER) ||
            (i + 2 < line.Size() && line[i].GetType() == TokenT::TYPE && line[i + 1].GetType() == TokenT::UNARY_OPERATOR) && line[i + 2].GetType() == TokenT::IDENTIFIER)
            return true;

    return false;
}

ehs::Vector<ehs::Byte> CompileIntoMachineCode(Compiler *compiler, const Language *lang, const ehs::Vector<Token> &tokens)
{
    ehs::Vector<Stack> stacks(1, 5);

    const ehs::Vector<ehs::Array<Token>> lines = SplitLines(tokens);
    for (ehs::UInt_64 i = 0; i < lines.Size(); ++i)
    {
        const ehs::Array<Token> line = lines[i];

        if (line[0].GetType() == TokenT::TYPE && line[1].GetType() == TokenT::IDENTIFIER)
        {
            if (line[2].GetType() == TokenT::ENCAPSULATOR && IsFunction({&line[3], line.Size() - 3}))
                compiler->AddSymbol({SymbolType::FUNCTION, line[1].GetValue(), 0, 0});
            else
                compiler->AddSymbol({SymbolType::VARIABLE, line[1].GetValue(), 0, 0});
        }
    }

    return {};
}