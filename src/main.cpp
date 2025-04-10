#include "arctyx/linker/formats/ELF.h"
#include "arctyx/linker/formats/ELF_Sym.h"

#include <ehs/EHS.h>
#include <ehs/io/Console.h>
#include <ehs/io/File.h>

int main()
{
	ehs::Initialize("Arctyx", "Alpha", {1, 0, 0});
	ehs::Log::EnableImmediateMode(true);

	ehs::Vector<ehs::Str_8> args = ehs::Console::GetArgs_8();
	if (args.Size() <= 1)
	{
		ehs::Console::Write_8(ehs::GetAppName_8() + " " + ehs::GetAppVersionId_8() + " v" + ehs::Str_8::FromNum(ehs::GetVersion().major) + "." +
			ehs::Str_8::FromNum(ehs::GetVersion().minor) + "." + ehs::Str_8::FromNum(ehs::GetVersion().patch));

		return 0;
	}

	ehs::File codeFile(args[1].RemoveAll("\""), ehs::Mode::READ, ehs::Disposition::OPEN);
	ehs::Serializer<ehs::UInt_64> code = codeFile.ReadSerializer_64(ehs::Endianness::LE, codeFile.Size());
	codeFile.Release();

	ELF executable(ELF_BIT_DEPTH_64, ELF_END_LE, ELFH_ABI_SYSTEMV, ELFH_TYPE_EXEC, ELF_ARCH_X86_64);

	ehs::UInt_64 lSgmtOffset = executable.GetLastSegmentOffset();

	ELF_Section text(".text", 0x1000, SECH_TYPE_PROGBITS, SECH_FLAG_ALLOC | SECH_FLAG_EXEC, 0x401000, 16);
	text.SetData(code);
	executable.AddSection(text);

	ELF_Section strTab(".strtab", 0, SECH_TYPE_STRTAB, 0, 0, 1);
	ehs::Serializer<ehs::UInt_64>& strTabData = strTab.GetData();

	constexpr ehs::Char_8 symName[] = "\0_start\0";
	strTabData.Resize(sizeof(symName));
	ehs::Util::Copy(&strTabData[0], symName, sizeof(symName));
	strTabData.SetOffset(sizeof(symName));

	executable.AddSection((ELF_Section&&)strTab);

	ELF_Section symTab(".symtab", 0, SECH_TYPE_SYMTAB, 0, 0, 8);
	symTab.SetLink(3);
	symTab.SetInfo(2);
	symTab.SetEntries(SYMH_SIZE);
	ehs::Serializer<ehs::UInt_64>& symTabData = symTab.GetData();

	ELF_Sym nullSym;
	nullSym.Serialize(symTabData);

	ELF_Sym _start(1, SYM_INFO(SYM_BIND_GLOBAL, SYM_TYPE_NOTYPE), SYM_VIS_DEFAULT, 2, 0x401000, 0);
	_start.Serialize(symTabData);

	executable.AddSection((ELF_Section&&)symTab);

	executable.AddProgram({PRGH_TYPE_LOAD, PRGH_FLAG_READ, 0, 0x400000, lSgmtOffset, 0, 0x1000});
	executable.AddProgram({PRGH_TYPE_LOAD, PRGH_FLAG_EXEC | PRGH_FLAG_READ, 0x1000, 0x401000, code.Size(), 0, 0x1000});

	executable.SetEntryPoint(1);

	ehs::File file(args[2].RemoveAll("\""), ehs::Mode::WRITE, ehs::Disposition::CREATE_PERSISTENT);
	file.WriteSerializer_64(executable.Serialize());

	ehs::Uninitialize();

	return 0;
}
