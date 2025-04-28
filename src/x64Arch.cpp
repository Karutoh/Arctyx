#include <ehs/Str.h>
#include <ehs/Version.h>
#include <ehs/Array.h>
#include <ehs/Serializer.h>

#include "arctyx/Arctyx.h"
#include "arctyx/compiler/Architecture.h"
#include "arctyx/compiler/Language.h"

bool StrContainsNum(const ehs::Str_8 &str)
{
    for (ehs::UInt_64 i = 0; i < str.Size(); ++i)
        if (str[i] >= '0' && str[i] <= '9')
            return true;

    return false;
}

ehs::Serializer<ehs::UInt_64> Assign(const Instruction *ins, const ehs::Byte *data)
{
    ehs::Serializer<ehs::UInt_64> result;

    const Architecture *arch = ins->GetArchitecture();

    const AssignBase *base = (AssignBase *)data;
    if (base->type == 1)
    {
        constexpr ehs::UInt_16 moveRegister = 0b1000100000000000;

        const AssignRR *params = (AssignRR *)data;
        const Register dst = *arch->GetRegister(params->dstReg);
        const Register src = *arch->GetRegister(params->srcReg);

        if (dst.GetByteDepth() == 1)
        {
            result.Write<ehs::UInt_16>(moveRegister | (params->isDstAddress ? 0b00000000 : 0b00000010) << 8
                | (params->isDstAddress || params->isSrcAddress ? 0b00000000 : 0b11000000) | dst.GetCode() << 3 | src.GetCode());
        }
        else if (dst.GetByteDepth() >= 2 && dst.GetByteDepth() <= 4)
        {
            result.Write<ehs::UInt_16>(moveRegister | (params->isDstAddress ? 0b00000001 : 0b00000011) << 8
                | (params->isDstAddress || params->isSrcAddress ? 0b00000000 : 0b11000000) | dst.GetCode() << 3 | src.GetCode());
        }
        else if (dst.GetByteDepth() == 8)
        {
            ehs::UInt_16 rex = 0b01001000;

            if (StrContainsNum(dst.GetName()))
                rex |= 0b00000001;

            if (StrContainsNum(src.GetName()))
                rex |= 0b00000100;

            result.Write<ehs::UInt_8>(rex);

            result.Write<ehs::UInt_16>(moveRegister | (params->isDstAddress ? 0b00000001 : 0b00000011) << 8
                | (params->isDstAddress || params->isSrcAddress ? 0b00000000 : 0b11000000) | dst.GetCode() << 3 | src.GetCode());
        }
    }
    else if (base->type == 2)
    {
        const AssignRI *params = (AssignRI *)data;
        const Register dst = *arch->GetRegister(params->dstReg);

        if (dst.GetByteDepth() == 1)
        {
            result.Write<ehs::UInt_16>(0b1011000000000000 | dst.GetCode() << 8 | (ehs::UInt_8)params->srcImm);
        }
        else if (dst.GetByteDepth() == 2)
        {
            result.Write<ehs::UInt_16>(0b0110011010110000);
            result.Write<ehs::UInt_16>(params->srcImm);
        }
        else if (dst.GetByteDepth() == 4)
        {
            result.Write<ehs::UInt_8>(0b10111000 | dst.GetCode());
            result.Write<ehs::UInt_32>(params->srcImm);
        }
        else if (dst.GetByteDepth() == 8)
        {
            result.Write<ehs::UInt_8>(0b0100100010111000 | (StrContainsNum(dst.GetName()) ? 0b00000001 : 0b00000000) << 8 | dst.GetCode());
            result.Write(params->srcImm);
        }
    }

    return result;
}

ehs::Version GetPluginVersion();

bool InitializePlugin()
{
    Architecture x64("x64");

    x64.AddRegister({"RAX", 8, 0b000});
    x64.AddRegister({"EAX", 4, 0b000});
    x64.AddRegister({"AX", 2, 0b000});
    x64.AddRegister({"AH", 1, 0b000});
    x64.AddRegister({"AL", 1, 0b000});

    x64.AddRegister({"RCX", 8, 0b001});
    x64.AddRegister({"ECX", 4, 0b001});
    x64.AddRegister({"CX", 2, 0b001});
    x64.AddRegister({"CH", 1, 0b001});
    x64.AddRegister({"CL", 1, 0b001});

    x64.AddRegister({"RDX", 8, 0b010});
    x64.AddRegister({"EDX", 4, 0b010});
    x64.AddRegister({"DX", 2, 0b010});
    x64.AddRegister({"DH", 1, 0b010});
    x64.AddRegister({"DL", 1, 0b010});

    x64.AddRegister({"RBX", 8, 0b011});
    x64.AddRegister({"EBX", 4, 0b011});
    x64.AddRegister({"BX", 2, 0b011});
    x64.AddRegister({"BH", 1, 0b011});
    x64.AddRegister({"BL", 1, 0b011});

    x64.AddRegister({"RSP", 8, 0b100});
    x64.AddRegister({"ESP", 4, 0b100});
    x64.AddRegister({"SP", 2, 0b100});
    x64.AddRegister({"SPL", 1, 0b100});

    x64.AddRegister({"RBP", 8, 0b101});
    x64.AddRegister({"EBP", 4, 0b101});
    x64.AddRegister({"BP", 2, 0b101});
    x64.AddRegister({"BPL", 1, 0b101});

    x64.AddRegister({"RSI", 8, 0b110});
    x64.AddRegister({"ESI", 4, 0b110});
    x64.AddRegister({"SI", 2, 0b110});
    x64.AddRegister({"SIL", 1, 0b110});

    x64.AddRegister({"RDI", 8, 0b111});
    x64.AddRegister({"EDI", 4, 0b111});
    x64.AddRegister({"DI", 2, 0b111});
    x64.AddRegister({"DIL", 1, 0b111});

    x64.AddRegister({"R8", 8, 0b000});
    x64.AddRegister({"R8D", 4, 0b000});
    x64.AddRegister({"R8W", 2, 0b000});
    x64.AddRegister({"R8B", 1, 0b000});

    x64.AddRegister({"R9", 8, 0b001});
    x64.AddRegister({"R9D", 4, 0b001});
    x64.AddRegister({"R9W", 2, 0b001});
    x64.AddRegister({"R9B", 1, 0b001});

    x64.AddRegister({"R10", 8, 0b010});
    x64.AddRegister({"R10D", 4, 0b010});
    x64.AddRegister({"R10W", 2, 0b010});
    x64.AddRegister({"R10B", 1, 0b010});

    x64.AddRegister({"R11", 8, 0b011});
    x64.AddRegister({"R11D", 4, 0b011});
    x64.AddRegister({"R11W", 2, 0b011});
    x64.AddRegister({"R11B", 1, 0b011});

    x64.AddRegister({"R12", 8, 0b100});
    x64.AddRegister({"R12D", 4, 0b100});
    x64.AddRegister({"R12W", 2, 0b100});
    x64.AddRegister({"R12B", 1, 0b100});

    x64.AddRegister({"R13", 8, 0b101});
    x64.AddRegister({"R13D", 4, 0b101});
    x64.AddRegister({"R13W", 2, 0b101});
    x64.AddRegister({"R13B", 1, 0b101});

    x64.AddRegister({"R14", 8, 0b110});
    x64.AddRegister({"R14D", 4, 0b110});
    x64.AddRegister({"R14W", 2, 0b110});
    x64.AddRegister({"R14B", 1, 0b110});

    x64.AddRegister({"R15", 8, 0b111});
    x64.AddRegister({"R15D", 4, 0b111});
    x64.AddRegister({"R15W", 2, 0b111});
    x64.AddRegister({"R15B", 1, 0b111});

    Instruction assign("Assign");
    assign.SetTranslation(Assign);

    Architecture::Add((Architecture &&)x64);

    return true;
}

ehs::Str_8 GetPluginName()
{
    return "x64 Architecture";
}

ehs::Version GetPluginVersion()
{
    return {1, 0, 0};
}

bool ShutdownPlugin()
{
    return true;
}