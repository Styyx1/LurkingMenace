#pragma once
#include "settings.h"

namespace LurkingMenaceMCM
{
	class Papyrus
	{
	public:
		static bool Register(RE::BSScript::IVirtualMachine* a_vm)
		{
			a_vm->RegisterFunction("GetVersion"sv, CLASS_NAME, GetVersion, true);
			a_vm->RegisterFunction("DoUpdateSettings"sv, CLASS_NAME, DoUpdateSettings);
			SKSE::log::info("Registered funcs for class {:s}"sv, CLASS_NAME);

			return true;
		}

	private:
		inline static constexpr auto CLASS_NAME{ "LurkingMenaceMCM"sv };

		enum
		{
			kVersion = 1
		};

		static std::int32_t GetVersion(RE::StaticFunctionTag*)
		{
			return kVersion;
		}

		static void DoUpdateSettings(RE::StaticFunctionTag*)
		{
			Settings* settings = Settings::GetSingleton();
			settings->LoadSettings();
		}
	};
}

