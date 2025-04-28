#pragma once

#include <ehs/Array.h>
#include <ehs/Types.h>
#include <ehs/system/Open.h>

class EHS_LIB_IO Arctyx
{
private:
	static ehs::Array<ehs::Open> plugins;

	static void LoadArchitecturePlugins();

	static void LoadLanguagePlugins();

public:
	static void Initialize();

	static void Uninitialize();
};