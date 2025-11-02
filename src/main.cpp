#include "cache.h"
#include "settings.h"
#include "formloader.h"
#include "events.h"
#include "ui.h"


void Listener(SKSE::MessagingInterface::Message *a_msg) noexcept
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		Forms::Loader::GetSingleton()->LoadForms(Config::Settings::debug_logging.GetValue());
		Config::JSONLoader::LoadExceptionsFromFolder();
		auto manager = Events::LootActivateEvent::GetSingleton();
		manager->RegisterActivateEvents();
		auto menuManager = Events::MenuEvent::GetSingleton();
		menuManager->RegisterMenuEvents();
		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface *a_skse)
{
	SKSE::Init(a_skse);
	
	Cache::CacheAddLibAddresses();
	SKSE::GetMessagingInterface()->RegisterListener(Listener);
	Config::Settings::GetSingleton()->UpdateSettings();
	UI::Register();
	return true;
}
