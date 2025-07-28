#include <ehs/Str.h>
#include <ehs/Version.h>

#include "arctyx/Arctyx.h"
#include "arctyx/compiler/Architecture.h"
#include "arctyx/compiler/Language.h"
#include "arctyx/compiler/Stack.h"
#include "arctyx/compiler/Symbol.h"
#include "arctyx/compiler/Compiler.h"

#include "arctyxLang/ArctyxCompiler.h"

ehs::Version GetPluginVersion();

bool InitializePlugin()
{
    const Architecture *x64 = Architecture::Get("x64");
    if (!x64)
        return false;

    Language arctyx("Arctyx", GetPluginVersion());

    arctyx.AddEOL('\n');
    arctyx.AddEOL(';');

    arctyx.AddPrimitive({"Bool", 1, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"Byte", 1, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"Char_8", 1, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"Char_16", 2, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"Char_32", 4, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"UInt_8", 1, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"UInt_16", 2, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"UInt_32", 4, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"UInt_64", 8, Signedness::UNSIGNED});
    arctyx.AddPrimitive({"SInt_8", 1, Signedness::SIGNED});
    arctyx.AddPrimitive({"SInt_16", 2, Signedness::SIGNED});
    arctyx.AddPrimitive({"SInt_32", 4, Signedness::SIGNED});
    arctyx.AddPrimitive({"SInt_64", 8, Signedness::SIGNED});
    arctyx.AddPrimitive({"Void", 0, Signedness::UNSIGNED});

    arctyx.AddOperator({"=", "Assign"});
    arctyx.AddOperator({"+", "Add"});
    arctyx.AddOperator({"-", "Subtract"});
    arctyx.AddOperator({"++", "Increment"});
    arctyx.AddOperator({"--", "Decrement"});
    arctyx.AddOperator({"*", "Multiply"});
    arctyx.AddOperator({"/", "Divide"});
    arctyx.AddOperator({"%", "Mod"});
    arctyx.AddOperator({"<", "Less Than"});
    arctyx.AddOperator({">", "Greater Than"});
    arctyx.AddOperator({"<=", "Less Than or Equal"});
    arctyx.AddOperator({">=", "Greater Than or Equal"});

    arctyx.AddKeyword("If");
    arctyx.AddKeyword("Elif");
    arctyx.AddKeyword("Else");
    arctyx.AddKeyword("While");
    arctyx.AddKeyword("For");
    arctyx.AddKeyword("Return");
    arctyx.AddKeyword("Break");
    arctyx.AddKeyword("Continue");
    arctyx.AddKeyword("Const");
    arctyx.AddKeyword("Class");
    arctyx.AddKeyword("Prot");
    arctyx.AddKeyword("Priv");
    arctyx.AddKeyword("Pub");
    arctyx.AddKeyword("Static");
    arctyx.AddKeyword("New");
    arctyx.AddKeyword("Del");
    arctyx.AddKeyword("DelArr");
    arctyx.AddKeyword("Enum");

    arctyx.SetCompileIntoTokensCb(CompileIntoTokens);

    arctyx.SetCompileIntoMachineCodeCb(CompileIntoMachineCode);

    Language::Add((Language &&)arctyx);

    return true;
}

ehs::Str_8 GetPluginName()
{
    return "Arctyx Language";
}

ehs::Version GetPluginVersion()
{
    return {1, 0, 0};
}

bool ShutdownPlugin()
{
    return true;
}