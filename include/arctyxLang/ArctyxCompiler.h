#pragma once

#include "arctyx/compiler/Compiler.h"

ehs::Vector<Token> CompileIntoTokens(Compiler *compiler, const Language *lang, const ehs::Str_8 &code);

ehs::Vector<ehs::Byte> CompileIntoMachineCode(Compiler *compiler, const Language *lang, const Architecture *arch, const ehs::Vector<Token> &tokens);