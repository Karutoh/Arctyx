#include "arctyx/Arctyx.h"

#include <ehs/Vector.h>
#include <ehs/system/CPU.h>

#include "arctyx/compiler/Architecture.h"
#include "arctyx/compiler/Language.h"
#include "arctyx/compiler/Token.h"

void Arctyx::Initialize()
{
}

void Arctyx::Uninitialize()
{
	for (ehs::Size i = 0; Architecture::architectures.Size(); ++i)
		delete Architecture::architectures[i];

	Architecture::architectures.Clear();

	for (ehs::UInt_64 i = 0; i < Language::languages.Size(); ++i)
		delete Language::languages[i];

	Language::languages.Clear();
}
