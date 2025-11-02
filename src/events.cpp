#include "events.h"
#include "settings.h"
#include "utility.h"
#include "formloader.h"

namespace Events
{
    void RegisterEvents()
    {
        LootActivateEvent::GetSingleton()->RegisterActivateEvents();
        MenuEvent::GetSingleton()->RegisterMenuEvents();
    }

    EventRes MenuEvent::ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
    { 
        auto loot_menu        = RE::ContainerMenu::MENU_NAME;

        if (!event)
            return EventRes::kContinue;

        if(!event->opening)
            return EventRes::kContinue;

        if (event->menuName == loot_menu) {
            if (Events::LootActivateEvent::was_activated) {
                MenuUtil::CloseMenu(loot_menu);
                Events::LootActivateEvent::was_activated = false;
            }
        }
        return EventRes::kContinue;
    };

    void LootActivateEvent::ProcessNPCEvent(RE::Actor* actor)
    {
        if (actor && actor->IsDead()) {
            if (Config::Settings::npc_spawn_werewolf.GetValue() && actor->IsInFaction(Forms::Loader::werewolf_faction)) {
                if (RandomiserUtil::IsPercentageChanceFloat(Config::Settings::mimic_chance.GetValue())) {
                    was_activated = true;
                    DelayedNPCSpawn(actor, Forms::Loader::npc_spawn_werewolf, Forms::Loader::spawn_visual_explosion, Utility::GetTimer());
                    return;
                }
            }
            else {
                auto chance = RandomiserUtil::GetRandomFloat(0.0, 100.0);
                if (chance <= Config::Settings::mimic_chance.GetValue()) {
                    was_activated = true;
                    DelayedNPCSpawn(actor, Forms::Loader::npc_spawn_generic, Forms::Loader::spawn_visual_explosion, Utility::GetTimer());
                    return;
                }
            }
        }
    }

    EventRes LootActivateEvent::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*)
    {
        using namespace Forms;
        using namespace Config;
        if (!a_event)
            return EventRes::kContinue;

        if (!a_event->actionRef || !a_event->objectActivated.get())
            return EventRes::kContinue;

        auto activated_object = a_event->objectActivated.get();
        RE::PlayerCharacter* player   = Cache::GetPlayerSingleton();
        if(!a_event->actionRef->IsPlayerRef())
            return EventRes::kContinue;

        bool        isLocked   = a_event->objectActivated->IsLocked();
        std::string nameOfCont = a_event->objectActivated->GetName();

        if(!Utility::isAnyException(activated_object)){
            if (Settings::npc_spawn_generic.GetValue() && activated_object->Is(RE::FormType::ActorCharacter)) {
                RE::Actor* dead_guy = a_event->objectActivated->As<RE::Actor>();
                // Only do stuff when looking at dead actors
                if (dead_guy && dead_guy->IsDead()) {
                    ProcessNPCEvent(dead_guy);
                }
                return EventRes::kContinue;
            }
            
            bool is_player_owned = activated_object->GetActorOwner() == player->GetActorBase();
            
            if (activated_object->GetBaseObject()->Is(RE::FormType::Container) && !is_player_owned && !isLocked) {                
                if(Settings::debug_logging.GetValue())
                    Utility::LogOwnership(activated_object);
                if (Settings::GetSingleton()->ContainerEventsActive()) {
                    auto ev_type = Utility::GetSpawnEvent(activated_object);  
                    if (ev_type == Utility::SpawnEvent::kNone) {
                        REX::WARN("NO SPAWN EVENT FOUND");
                        return EventRes::kContinue;
                    }

                    if (!RandomiserUtil::IsPercentageChanceFloat(Config::Settings::mimic_chance.GetValue())) {
                        return EventRes::kContinue;
                    }

                    was_activated = true;
                    REX::INFO("made it past the returns spawn event type is {}", Utility::SpawnEventToString(ev_type));
                    switch (ev_type) {

                    case Utility::SpawnEvent::kDraugr:
                        DelayedContainerSpawn(activated_object, Forms::Loader::container_spawn_draugr, Forms::Loader::spawn_visual_explosion, Utility::GetTimer());
                        return EventRes::kContinue;
                        break;
                    case Utility::SpawnEvent::kUrn:
                        activated_object->PlaceObjectAtMe(Forms::Loader::spawn_urn_explosion, false);
                        Utility::ApplyStress(player);
                        std::jthread([=] {
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            SKSE::GetTaskInterface()->AddTask([=] {
                                was_activated = false;
                                REX::INFO("set activated to false");
                                });
                            }).detach();
                        return EventRes::kContinue;
                        break;
                    case Utility::SpawnEvent::kDwarven:
                        DelayedContainerSpawn(activated_object, Forms::Loader::container_spawn_dwarven, Forms::Loader::spawn_visual_explosion, Utility::GetTimer());
                        return EventRes::kContinue;
                        break;
                    case Utility::SpawnEvent::kWarlock:
                        DelayedContainerSpawn(activated_object, Forms::Loader::container_spawn_warlock, Forms::Loader::spawn_visual_explosion, Utility::GetTimer());
                        return EventRes::kContinue;
                        break;
                    case Utility::SpawnEvent::kGeneric:
                        DelayedContainerSpawn(activated_object, Forms::Loader::container_spawn_mimic, Forms::Loader::spawn_visual_explosion, Utility::GetTimer());
                        return EventRes::kContinue;
                        break;
                    default:
                        return EventRes::kContinue;
                        break;
                    }
                }
            }            
        }

        return EventRes::kContinue;
    }

    

    void LootActivateEvent::DelayedContainerSpawn(RE::TESObjectREFR* a_eventItem, RE::TESNPC* a_enemyToSpawn, RE::TESBoundObject* a_explosion,
                                                  std::chrono::duration<double> a_threadDelay)
    {
        bool explosion_visuals_active = Config::Settings::visual_explosions_for_spawns_active.GetValue();
        bool delayed_explosion_active = Config::Settings::delay_explosion_active.GetValue();
        RE::PlayerCharacter* player        = Cache::GetPlayerSingleton();
        if (!delayed_explosion_active) {
            if (explosion_visuals_active) {
                a_eventItem->PlaceObjectAtMe(a_explosion, false);
            }
        }
        std::jthread([=] {
            std::this_thread::sleep_for(a_threadDelay);
            SKSE::GetTaskInterface()->AddTask([=] {
                if (delayed_explosion_active) {
                    if (explosion_visuals_active) {
                        a_eventItem->PlaceObjectAtMe(a_explosion, false);
                    }
                }
                auto mimic = a_eventItem->PlaceObjectAtMe(a_enemyToSpawn, false)->AsReference();
                mimic->DoMoveToHigh();
                mimic->MoveTo(a_eventItem);
                Utility::RemoveAllItems(a_eventItem, mimic);
                Utility::PlayMeme();
                Utility::ApplyStress(player);
                a_eventItem->Disable();
            });
        }).detach();
        std::jthread([=] {
            std::this_thread::sleep_for(a_threadDelay + std::chrono::seconds(10));
            SKSE::GetTaskInterface()->AddTask([=] { a_eventItem->Enable(false); });
        }).detach();
        std::jthread([=] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            SKSE::GetTaskInterface()->AddTask([=] { was_activated = false; });
        }).detach();
    }

    void LootActivateEvent::DelayedNPCSpawn(RE::TESObjectREFR* a_eventItem, RE::TESNPC* a_enemyToSpawn, RE::TESBoundObject* a_explosion,
                                            std::chrono::duration<double> a_threadDelay)
    {
        bool delay_active = Config::Settings::delay_explosion_active.GetValue();
        bool explosion_visuals = Config::Settings::visual_explosions_for_spawns_active.GetValue();
        RE::PlayerCharacter* player   = Cache::GetPlayerSingleton();

        if (!delay_active) {
            if (explosion_visuals) {
                a_eventItem->PlaceObjectAtMe(a_explosion, false);
            }
        }
        std::jthread([=] {
            std::this_thread::sleep_for(a_threadDelay);
            SKSE::GetTaskInterface()->AddTask([=] {
                if (delay_active) {
                    if (explosion_visuals) {
                        a_eventItem->PlaceObjectAtMe(a_explosion, false);
                    }
                }
                auto dude = a_eventItem->PlaceObjectAtMe(a_enemyToSpawn, false)->AsReference();
                dude->DoMoveToHigh();
                dude->MoveTo(a_eventItem);
                Utility::PlayMeme();
                Utility::ApplyStress(player);
            });
        }).detach();
        std::jthread([=] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            SKSE::GetTaskInterface()->AddTask([=] { was_activated = false; });
        }).detach();
    }
    
} // namespace Events
