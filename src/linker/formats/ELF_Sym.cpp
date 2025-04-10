#include "arctyx/linker/formats/ELF_Sym.h"

ELF_Sym::ELF_Sym()
	: nameOffset(0), type(0), visibility(0), section(0), value(0), size(0)
{

}

ELF_Sym::ELF_Sym(ehs::UInt_32 nameOffset, ehs::UInt_8 type, ehs::UInt_8 visibility, ehs::UInt_16 section, ehs::UInt_64 value, ehs::UInt_64 size)
	: nameOffset(nameOffset), type(type), visibility(visibility), section(section), value(value), size(size)
{
}

void ELF_Sym::Serialize(ehs::Serializer<ehs::UInt_64>& inData) const
{
	inData.Write(nameOffset);
	inData.Write(type);
	inData.Write(visibility);
	inData.Write(section);
	inData.Write(value);
	inData.Write(size);
}

