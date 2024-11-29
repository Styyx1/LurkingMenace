#include "cache.h"
#include "events.h"
#include "logging.h"
#include "SKSE/Interfaces.h"
#include "settings.h"

void Listener(SKSE::MessagingInterface::Message *message) noexcept
{
	if (message->type == SKSE::MessagingInterface::kDataLoaded)
	{
		auto settings = Settings::GetSingleton();
		settings->LoadSettings();
		settings->LoadForms();
		auto manager = Events::LootActivateEvent::GetSingleton();
		manager->RegisterActivateEvents();
		auto menuManager = Events::MenuEvent::GetSingleton();
		menuManager->RegisterMenuEvents();
	}
}

SKSEPluginLoad(const SKSE::LoadInterface *a_skse)
{
	InitializeLogging();

	const auto plugin{SKSE::PluginDeclaration::GetSingleton()};
	const auto version{plugin->GetVersion()};

	logger::info("{} {} is loading...", plugin->GetName(), version);

	SKSE::Init(a_skse);
	Cache::CacheAddLibAddresses();

	if (const auto messaging{SKSE::GetMessagingInterface()}; !messaging->RegisterListener(Listener))
		return false;

	logger::info("{} has finished loading.", plugin->GetName());

	return true;
}
