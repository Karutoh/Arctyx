#include "arctyxLang/ArctyxCompiler.h"

#include <ehs/io/Console.h>

#include "arctyx/compiler/Scope.h"

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

bool IsPrimitive(const ehs::Array<Primitive> &primitives, const ehs::Str_8 &value)
{
    for (ehs::UInt_64 i = 0; i < primitives.Size(); ++i)
        if (value == primitives[i].GetName())
            return true;

    return false;
}

bool IsKeyword(const ehs::Array<ehs::Str_8> &keywords, const ehs::Str_8 &value)
{
	const ehs::Str_8 loweredVal = value.GetLower();

	for (ehs::UInt_64 i = 0; i < keywords.Size(); ++i)
		if (loweredVal == keywords[i])
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

bool IsEncapsulatorStart(const ehs::Char_8* c)
{
	switch (*c)
	{
	case '(':
		return true;
	case '{':
		return true;
	case '[':
		return true;
	default:
		return false;
	}
}

bool IsEncapsulatorEnd(const ehs::Char_8* c)
{
	if (*c == '}')
		return true;

	return false;
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

bool ParseText(const ehs::UInt_64 &line, ehs::Vector<Token> &tokens, const ehs::Array<ehs::Char_8> &eols,
    const ehs::Array<Primitive> &primitives, const ehs::Array<ehs::Str_8> &keywords, ehs::Char_8 **start,
    ehs::Char_8** i)
{
	const bool isEOL = IsEOL(eols, *i);
	const bool isSpace = **i == ' ';
	const bool isEnd = *i != *start && !IsAlphaNumeric(*i);

    if (isEOL || isSpace || isEnd)
    {
        ehs::Str_8 text(*start, *i - *start);

        if (IsPrimitive(primitives, text))
            tokens.Push({TokenT::TYPE, text});
        else if (IsKeyword(keywords, text))
            tokens.Push({TokenT::KEYWORD, text});
        else if (text == "True" || text == "False")
            tokens.Push({TokenT::BOOLEAN, text});
        else if (tokens.Size())
        {
        	if (tokens[tokens.End()].GetType() == TokenT::IDENTIFIER)
        		tokens[tokens.End()].SetType(TokenT::TYPE);

	        tokens.Push({TokenT::IDENTIFIER, text});
        }
        else
    	{
    		ehs::Console::Write_8("Error Line " + ehs::Str_8::FromNum(line) + ": " + text);

        	return false;
    	}

        if (isEOL)
            tokens.Push({TokenT::EOL, {*i, 1}});
        else if (!isSpace && isEnd)
        {
            *start = *i;

            return true;
        }

        *start = *i + 1;
    }

    ++*i;

	return true;
}

ehs::Vector<Token> CompileIntoTokens(Compiler *compiler, const Language *lang, const ehs::Str_8 &code)
{
	ehs::UInt_64 line = 0;

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
        {
	        if (!ParseText(line, tokens, eols, primitives, keywords, &start, &i))
	        	return {};
        }
        else if (*start == ',')
        {
            tokens.Push({TokenT::SEPARATOR, {i++, 1}});
            start = i;
        }
        else if (IsEOL(eols, start))
        {
            if (tokens[tokens.End()].GetType() != TokenT::EOL)
            {
            	++line;
	            tokens.Push({TokenT::EOL, {i, 1}});
            }

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
	if (!line.Size())
		return true;

    for (ehs::UInt_64 i = 0; i < line.Size(); ++i)
        if ((line[i].GetType() == TokenT::SEPARATOR && line[i].GetValue() == ",") ||
            (i + 1 < line.Size() && line[i].GetType() == TokenT::TYPE && line[i + 1].GetType() == TokenT::IDENTIFIER) ||
            (i + 2 < line.Size() && line[i].GetType() == TokenT::TYPE && line[i + 1].GetType() == TokenT::UNARY_OPERATOR) && line[i + 2].GetType() == TokenT::IDENTIFIER)
            return true;

    return false;
}

void ParseGlobalSymbols(Compiler *compiler, const Language *lang, const ehs::Vector<ehs::Array<Token>> &lines)
{
	ehs::UInt_64 encapsulators = 0;
	Scope rootScope;
	Scope *currentScope = &rootScope;

	for (ehs::UInt_64 i = 0; i < lines.Size(); ++i)
	{
		const ehs::Array<Token> line = lines[i];
		for (ehs::UInt_64 l = 0; l < line.Size(); ++l)
		{
			if (IsEncapsulatorStart(line[l].GetValue()))
			{
				encapsulators++;
			}
			else if (IsEncapsulatorEnd(line[l].GetValue()))
			{
				encapsulators--;

				if (currentScope->GetParent())
				{
					currentScope = currentScope->GetParent();
				}
				else
				{
					rootScope = {};
					currentScope = &rootScope;
				}
			}
		}

		if (line.Size() < 2)
			continue;

		if (line[0].GetType() == TokenT::TYPE && line[1].GetType() == TokenT::IDENTIFIER)
		{
			//Checks if is function or variable.
			if (line[2].GetType() == TokenT::ENCAPSULATOR && line[2].GetValue() == "(" &&
				line[line.End()].GetType() == TokenT::ENCAPSULATOR && line[line.End()].GetValue() == ")" &&
				IsFunction({&line[3], line.Size() - 4}))
			{
				if (currentScope->IsValid())
					compiler->AddSymbol({SymbolType::FUNCTION, currentScope->ToStr() + line[1].GetValue(), i});
				else
					compiler->AddSymbol({SymbolType::FUNCTION, line[1].GetValue(), i});

				if (!rootScope.IsValid())
				{
					rootScope = Scope(line[1].GetValue(), ScopeType::FUNCTION);
				}
				else
				{
					currentScope = currentScope->AddChild(line[1].GetValue(), ScopeType::FUNCTION);
				}
			}
			else
			{
				if (currentScope->IsValid())
				{
					if (currentScope->GetType() != ScopeType::FUNCTION && currentScope->GetType() != ScopeType::METHOD && currentScope->GetType() != ScopeType::CONSTRUCTOR)
						compiler->AddSymbol({SymbolType::VARIABLE, currentScope->ToStr() + line[1].GetValue(), i});
				}
				else
					compiler->AddSymbol({SymbolType::VARIABLE, line[1].GetValue(), i});
			}
		}
		else if (line[line.End() - 1].GetType() == TokenT::KEYWORD && line[line.End()].GetType() == TokenT::IDENTIFIER)
		{
			if (line[0].GetValue().GetLower() == "namespace")
			{
				if (!rootScope.IsValid())
				{
					rootScope = Scope(line[1].GetValue(), ScopeType::NAMESPACE);
				}
				else
				{
					currentScope = currentScope->AddChild(line[1].GetValue(), ScopeType::NAMESPACE);
				}
			}
			else if (line[0].GetValue().GetLower() == "class")
			{
				if (currentScope->IsValid())
					compiler->AddSymbol({SymbolType::CLASS, currentScope->ToStr() + line[1].GetValue(), i});
				else
					compiler->AddSymbol({SymbolType::CLASS, line[1].GetValue(), i});

				if (!rootScope.IsValid())
				{
					rootScope = Scope(line[1].GetValue(), ScopeType::CLASS);
				}
				else
				{
					currentScope = currentScope->AddChild(line[1].GetValue(), ScopeType::CLASS);
				}
			}
			else if (line[0].GetValue().GetLower() == "struct")
			{
				if (currentScope->IsValid())
					compiler->AddSymbol({SymbolType::STRUCTURE, currentScope->ToStr() + line[1].GetValue(), i});
				else
					compiler->AddSymbol({SymbolType::STRUCTURE, line[1].GetValue(), i});

				if (!rootScope.IsValid())
				{
					rootScope = Scope(line[1].GetValue(), ScopeType::STRUCTURE);
				}
				else
				{
					currentScope = currentScope->AddChild(line[1].GetValue(), ScopeType::STRUCTURE);
				}
			}
		}
		else if (currentScope->GetType() == ScopeType::CLASS && line[0].GetType() == TokenT::IDENTIFIER && IsFunction({&line[1], line.Size() - 1}))
		{
			if (currentScope->IsValid())
				compiler->AddSymbol({SymbolType::CONSTRUCTOR, currentScope->ToStr() + line[0].GetValue(), i});
			else
				compiler->AddSymbol({SymbolType::CONSTRUCTOR, line[0].GetValue(), i});

			if (!rootScope.IsValid())
			{
				rootScope = Scope(line[0].GetValue(), ScopeType::CONSTRUCTOR);
			}
			else
			{
				currentScope = currentScope->AddChild(line[0].GetValue(), ScopeType::CONSTRUCTOR);
			}
		}
	}
}

void CompileGlobalSymbol(Compiler *compiler, const Language *lang, const Architecture *arch, Symbol *symbol, const ehs::Vector<ehs::Array<Token>> &lines)
{
	static ehs::Byte zero[] = {0, 0, 0, 0, 0, 0, 0, 0};
	ehs::Vector<ehs::Byte> machineCode;

	if (symbol->GetType() == SymbolType::VARIABLE)
	{
		Section *dataSect = compiler->GetSection(".data");

		ehs::Array<Token> line = lines[symbol->GetLine()];

		const Primitive *primitive = lang->GetPrimitive(line[0].GetValue());
		if (primitive)
		{
			if (line.Size() == 4)
			{
				ehs::UInt_8 byteDepth = primitive->GetByteDepth();
				ehs::UInt_8 padding = (byteDepth - (dataSect->Size() % byteDepth)) % byteDepth;
				if (padding)
					dataSect->Push(zero, padding);

				symbol->SetAddress(dataSect->Size());
				symbol->SetSize(primitive->GetByteDepth());

				ehs::Str_8 value = line[3].GetValue();

				if (value.IsNum())
				{
					ehs::UInt_64 data = value.ToDecimal<ehs::UInt_64>();

					dataSect->Push((ehs::Byte *)&data, byteDepth);
				}
				else if (value.GetLower() == "true")
					dataSect->Push(1);
				else if (value.GetLower() == "false")
					dataSect->Push(0);
				else if (value[0] == '\"' && value[value.Size() - 1] == '\"')
				{
					dataSect->Push((ehs::Byte *)&value[1], value.Size() - 2);
					dataSect->Push(0);
				}
			}
			else
			{

			}
		}
	}
}

ehs::Vector<ehs::Byte> CompileIntoMachineCode(Compiler *compiler, const Language *lang, const Architecture *arch, const ehs::Vector<Token> &tokens)
{
    ehs::Vector<Stack> stacks(1, 5);

	const ehs::Vector<ehs::Array<Token>> lines = SplitLines(tokens);

	ParseGlobalSymbols(compiler, lang, lines);

	const ehs::Array<Symbol> *symbols = compiler->GetSymbols();
	compiler->AddSection({".data", 8});
	compiler->AddSection({".class_meta", 8});
	compiler->AddSection({".text", 16});

	for (ehs::UInt_64 i = 0; i < symbols->Size(); ++i)
	{
		CompileGlobalSymbol(compiler, lang, arch, &(*symbols)[i], lines);
	}

	/*
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
    */

    return {};
}