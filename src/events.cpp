#include "events.h"
#include "formloader.h"
#include "settings.h"
#include "utility.h"

namespace Events
{
void RegisterEvents()
{
    LootActivateEvent::GetSingleton()->RegisterActivateEvents();
	HitEvent::GetSingleton()->Register();
    //MenuEvent::GetSingleton()->RegisterMenuEvents();
}
#pragma region Menu
EventRes MenuEvent::ProcessEvent(const RE::MenuOpenCloseEvent *event, RE::BSTEventSource<RE::MenuOpenCloseEvent> *)
{
    auto loot_menu = RE::ContainerMenu::MENU_NAME;

    if (!event)
        return EventRes::kContinue;

    if (!event->opening)
        return EventRes::kContinue;

    if (event->menuName == loot_menu)
    {
        if (Events::LootActivateEvent::check_timer.IsRunning())
        {
            MenuUtil::CloseMenu(loot_menu);
        }
    }
    return EventRes::kContinue;
};
#pragma endregion Menu
#pragma region Loot
EventRes LootActivateEvent::ProcessEvent(const RE::TESActivateEvent *a_event,
                                         RE::BSTEventSource<RE::TESActivateEvent> *)
{
    using namespace Forms;
    using namespace Config;

    if (!a_event || !a_event->actionRef || !a_event->objectActivated.get())
        return EventRes::kContinue;

    auto activated_object = a_event->objectActivated.get();
    RE::PlayerCharacter *player = Cache::GetPlayerSingleton();
    if (!a_event->actionRef->IsPlayerRef())
        return EventRes::kContinue;

	auto delay = Utility::GetTimer();

	ref_timer.StopIfExpired( activated_object, delay );

	if ( ref_timer.IsActive( activated_object ) )
		return EventRes::kContinue;

    if (!RandomiserUtil::IsPercentageChanceFloat(Config::Settings::mimic_chance.GetValue()))
        return EventRes::kContinue;
    if (CellUtil::IsJail(ActorUtil::GetPlayerCell(player)))
    {
        REX::INFO("location is jail");
        return EventRes::kContinue;
    }

    if (Utility::isAnyException(activated_object))
        return EventRes::kContinue;

    if (activated_object->GetBaseObject()->Is(RE::FormType::Activator))
    {
        if (const auto ore_key = RE::TESForm::LookupByEditorID<RE::BGSKeyword>(ORE_KEYWORD);
            ore_key && activated_object->HasKeyword(ore_key))
        {
            ref_timer.Start(activated_object);
            DelayedSpawn(player, Forms::Loader::npc_spawn_generic, Forms::Loader::spawn_visual_explosion,
                         Utility::GetTimer(), false);
            return EventRes::kContinue;
        };
    }

    auto spawn_type = Utility::GetSpawnEvent(activated_object);
    REX::INFO("Spawn event is: {}", Utility::SpawnEventToString(spawn_type));

    if (!Utility::IsSpawnTypeEnabled(spawn_type))
        return EventRes::kContinue;

    if (spawn_type == Utility::SpawnEvent::kNone)
        return EventRes::kContinue;

    if (activated_object->Is(RE::FormType::ActorCharacter))
    {
        RE::Actor *dead_guy = activated_object->As<RE::Actor>();
        if (dead_guy && dead_guy->IsDead())
        {
            auto npc_to_spawn = Utility::GetNPCFromSpawnType(spawn_type);
            if (npc_to_spawn)
            {
				ref_timer.Start( activated_object );
                DelayedSpawn(dead_guy, npc_to_spawn, Forms::Loader::spawn_visual_explosion, Utility::GetTimer(), false);
            }
        }
        return EventRes::kContinue;
    }

    // handle containers
    bool is_player_owned = activated_object->GetActorOwner() == player->GetActorBase();
    if (!activated_object->GetBaseObject()->Is(RE::FormType::Container) || is_player_owned ||
        activated_object->IsLocked())
        return EventRes::kContinue;

    if (!Settings::GetSingleton()->ContainerEventsActive())
        return EventRes::kContinue;

    // special case for Urns
    if (spawn_type == Utility::SpawnEvent::kUrn)
    {
		if ( !ref_timer.IsActive( activated_object ) )
        {
			ref_timer.Start( activated_object );
            activated_object->PlaceObjectAtMe(Forms::Loader::spawn_urn_explosion, false);
            Utility::ApplyStress(player);
        }
        return EventRes::kContinue;
    }
    auto npc_to_spawn = Utility::GetNPCFromSpawnType(spawn_type);
    if (npc_to_spawn)
    {
		ref_timer.Start( activated_object );
        bool disable_source = spawn_type <= Utility::SpawnEvent::kGeneric;
        DelayedSpawn(activated_object, npc_to_spawn, Forms::Loader::spawn_visual_explosion, Utility::GetTimer(),
                     disable_source);
    }
    return EventRes::kContinue;
}

void LootActivateEvent::DelayedSpawn(RE::TESObjectREFR *source, RE::TESNPC *npc_to_spawn, RE::TESBoundObject *explosion,
                                     std::chrono::duration<double> delay, bool disable_source)
{
    bool delay_active = Config::Settings::delay_explosion_active.GetValue();
    bool explosion_visuals = Config::Settings::visual_explosions_for_spawns_active.GetValue();
    RE::PlayerCharacter *player = Cache::GetPlayerSingleton();

    if (!delay_active && explosion_visuals)
    {
        source->PlaceObjectAtMe(explosion, false);
    }
    std::jthread([=] {
        std::this_thread::sleep_for(delay);
        SKSE::GetTaskInterface()->AddTask([=] {
            if (delay_active && explosion_visuals)
            {
                source->PlaceObjectAtMe(explosion, false);
            }
            auto spawned_npc = Utility::PapyrusPlaceAtMe(source, npc_to_spawn, true, false);
            ;
            spawned_npc->DoMoveToHigh();
            spawned_npc->MoveTo(source);

            if (disable_source)
            {
                Utility::RemoveAllItems(source, spawned_npc);
                source->Disable();
            }
            Utility::PlayMeme();
            Utility::ApplyStress(player);
        });
    }).detach();
    if (disable_source)
    {
        std::jthread([=] {
            std::this_thread::sleep_for(delay + std::chrono::seconds(10));
            SKSE::GetTaskInterface()->AddTask([=] { source->Enable(false); });
        }).detach();
    }
}
#pragma endregion Loot
EventRes HitEvent::ProcessEvent( const RE::TESHitEvent *event, RE::BSTEventSource<RE::TESHitEvent> * )
{
    using HitFlag = RE::TESHitEvent::Flag;
	if ( !event )
		return EventRes::kContinue;

    if ( !event->target || !event->cause || !event->source )
		return EventRes::kContinue;

    auto aggressor = event->cause ? event->cause->As<RE::Actor>() : nullptr;
	if ( !aggressor )
        return EventRes::kContinue;

    if ( aggressor->IsPlayerRef() )
		return EventRes::kContinue;

    if ( event->flags.any( HitFlag::kBashAttack ) )
		return EventRes::kContinue;
	auto targ = event->target.get();
	if ( !targ )
		return EventRes::kContinue;

    if ( !RefUtil::IsRefOreVein( targ ) )
		return EventRes::kContinue;

    auto weap = ActorUtil::getWieldingWeapon( aggressor );
	if ( !weap || weap->IsHandToHandMelee() || weap->IsRanged() )
		return EventRes::kContinue;

	auto key = RE::TESForm::LookupByEditorID<RE::BGSKeyword>( "VendorItemTool" );
    if (!key || !weap->HasKeyword(key))
		return EventRes::kContinue;

    if ( !Forms::Loader::ore_tool_list.contains( weap ) )
		return EventRes::kContinue;

    auto delay = Utility::GetTimer();

	LootActivateEvent::ref_timer.StopIfExpired( targ, delay );

    if ( LootActivateEvent::ref_timer.IsActive(targ) )
		return EventRes::kContinue;

    if ( targ->GetBaseObject()->Is( RE::FormType::Activator ) )
	{
		if ( const auto ore_key = RE::TESForm::LookupByEditorID<RE::BGSKeyword>( ORE_KEYWORD ); ore_key && targ->HasKeyword( ore_key ) )
		{
			LootActivateEvent::ref_timer.Start( targ );
			LootActivateEvent::GetSingleton()->DelayedSpawn( aggressor, Forms::Loader::npc_spawn_generic, Forms::Loader::spawn_visual_explosion, Utility::GetTimer(), false );
			return EventRes::kContinue;
		};
	}



	return EventRes::kContinue;
}
inline RE::UI_MESSAGE_RESULTS ContainerMenuHook::ProcessMessage(RE::ContainerMenu* a_this, RE::UIMessage& a_message)
{
	if ( a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow )
	{
		auto ref = a_this->GetTargetRefHandle();
		auto refr = RE::TESObjectREFR::LookupByHandle( ref );

		REX::INFO( "inside menu hook" );
		if ( refr.get() )
		{
			if ( LootActivateEvent::ref_timer.IsActive( refr.get() ) )
			{
				return RE::UI_MESSAGE_RESULTS::kIgnore;
			}
		}
    }
	
   
    return _Hook0(a_this, a_message);
}
} // namespace Events
