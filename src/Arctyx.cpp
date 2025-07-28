#include "arctyx/Arctyx.h"

#include <ehs/Vector.h>
#include <ehs/Version.h>
#include <ehs/system/CPU.h>
#include <ehs/system/Open.h>
#include <ehs/io/Directory.h>
#include <ehs/io/Console.h>

#include "arctyx/compiler/Architecture.h"
#include "arctyx/compiler/Language.h"
#include "arctyx/compiler/Token.h"

ehs::Array<ehs::Open> Arctyx::plugins;

typedef bool (*InitializePlugin)();
typedef ehs::Str_8 (*GetPluginName)();
typedef ehs::Version (*GetPluginVersion)();
typedef bool (*ShutdownPlugin)();

void Arctyx::LoadArchitecturePlugins()
{
	#if defined(EHS_OS_LINUX)
		const ehs::Str_8 delimeter = ".so";
	#elif defined(EHS_OS_WINDOWS)
		const ehs::Str_8 delimeter = ".dll";
	#endif

	ehs::Array<ehs::Str_8> files = ehs::Directory::GetAllFiles("plugins/architectures");
	for (ehs::UInt_64 i = 0; i < files.Size(); ++i)
	{
		if (!files[i].Find(delimeter, nullptr, ehs::SearchPattern::RIGHT_LEFT))
			continue;

		ehs::Open plugin("plugins/architectures/" + files[i]);
		InitializePlugin initFunc = (InitializePlugin)plugin.Retrieve("_Z16InitializePluginv");
		GetPluginName nameFunc = (GetPluginName)plugin.Retrieve("_Z13GetPluginNamev");
		GetPluginVersion verFunc = (GetPluginVersion)plugin.Retrieve("_Z16GetPluginVersionv");

		ehs::Str_8 pluginName = nameFunc();
		ehs::Version pluginVersion = verFunc();

		if (!initFunc())
		{
			ehs::Console::Write_8("Failed to initialize plugin " + pluginName + " v" + pluginVersion.major + "." +
				pluginVersion.minor + "." + pluginVersion.patch);

			continue;
		}

		ehs::Console::Write_8("Successfully initialized plugin " + pluginName + " v" + pluginVersion.major + "." +
			pluginVersion.minor + "." + pluginVersion.patch);

		plugins.Push((ehs::Open &&)plugin);
	}
}

void Arctyx::LoadLanguagePlugins()
{
	#if defined(EHS_OS_LINUX)
		const ehs::Str_8 delimeter = ".so";
	#elif defined(EHS_OS_WINDOWS)
		const ehs::Str_8 delimeter = ".dll";
	#endif

	ehs::Array<ehs::Str_8> files = ehs::Directory::GetAllFiles("plugins/languages");
	for (ehs::UInt_64 i = 0; i < files.Size(); ++i)
	{
		if (!files[i].Find(delimeter, nullptr, ehs::SearchPattern::RIGHT_LEFT))
			continue;

		ehs::Open plugin("plugins/languages/" + files[i]);
		InitializePlugin initFunc = (InitializePlugin)plugin.Retrieve("_Z16InitializePluginv");
		GetPluginName nameFunc = (GetPluginName)plugin.Retrieve("_Z13GetPluginNamev");
		GetPluginVersion verFunc = (GetPluginVersion)plugin.Retrieve("_Z16GetPluginVersionv");

		ehs::Str_8 pluginName = nameFunc();
		ehs::Version pluginVersion = verFunc();

		if (!initFunc())
		{
			ehs::Console::Write_8("Failed to load plugin " + pluginName + " v" + pluginVersion.major + "." +
				pluginVersion.minor + "." + pluginVersion.patch);

			continue;
		}

		ehs::Console::Write_8("Successfully loaded plugin " + pluginName + " v" + pluginVersion.major + "." +
			pluginVersion.minor + "." + pluginVersion.patch);

		plugins.Push((ehs::Open &&)plugin);
	}
}

void Arctyx::Initialize()
{
	ehs::Console::Write_8("Loading plugins...");

	LoadArchitecturePlugins();
	LoadLanguagePlugins();

	ehs::Console::Write_8("");
}

void Arctyx::Uninitialize()
{
	for (ehs::Size i = 0; i < Architecture::architectures.Size(); ++i)
		delete Architecture::architectures[i];

	Architecture::architectures.Clear();

	for (ehs::UInt_64 i = 0; i < Language::languages.Size(); ++i)
		delete Language::languages[i];

	Language::languages.Clear();

	ehs::Console::Write_8("\nUnloading plugins...");

	for (ehs::UInt_64 i = 0; i < plugins.Size(); ++i)
	{
		GetPluginName nameFunc = (GetPluginName)plugins[i].Retrieve("_Z13GetPluginNamev");
		GetPluginVersion verFunc = (GetPluginVersion)plugins[i].Retrieve("_Z16GetPluginVersionv");
		ShutdownPlugin shutFunc = (ShutdownPlugin)plugins[i].Retrieve("_Z14ShutdownPluginv");

		ehs::Str_8 pluginName = nameFunc();
		ehs::Version pluginVersion = verFunc();

		if (!shutFunc)
		{
			ehs::Console::Write_8("Failed to unload plugin " + pluginName + " v" + pluginVersion.major + "." +
				pluginVersion.minor + "." + pluginVersion.patch);
		}

		ehs::Console::Write_8("successfully unloaded plugin " + pluginName + " v" + pluginVersion.major + "." +
				pluginVersion.minor + "." + pluginVersion.patch);
	}

	plugins.Clear();
}
