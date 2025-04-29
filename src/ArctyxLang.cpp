#include <ehs/Str.h>
#include <ehs/Version.h>

#include "arctyx/Arctyx.h"
#include "arctyx/compiler/Architecture.h"
#include "arctyx/compiler/Language.h"

ehs::Version GetPluginVersion();

bool InitializePlugin()
{
    const Architecture *x64 = Architecture::Get("x64");
    if (!x64)
        return false;

    Language arctyx("Arctyx", GetPluginVersion());

    arctyx.AddSeparator(',');

    arctyx.AddEOL('\n');
    arctyx.AddEOL(';');

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

    arctyx.AddOperator({"=", "Assign"});

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