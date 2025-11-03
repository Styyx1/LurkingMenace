#pragma once
#include "settings.h"

namespace Events
{
    using EventRes = RE::BSEventNotifyControl;
    void RegisterEvents();

    struct LootActivateEvent : public REX::Singleton<LootActivateEvent>, public RE::BSTEventSink<RE::TESActivateEvent>
    {
        EventRes ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*);

        void RegisterActivateEvents()
        {
            if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton()) {
                scripts->AddEventSink<RE::TESActivateEvent>(this);
                REX::INFO("Registered {}", typeid(RE::TESActivateEvent).name());
            }
        }

        inline static TimerUtil::Timer check_timer;
        void DelayedSpawn(RE::TESObjectREFR* source, RE::TESNPC* npc_to_spawn, RE::TESBoundObject* explosion, std::chrono::duration<double> delay,bool disable_source = false);
    };

    struct MenuEvent : public REX::Singleton<MenuEvent>, public RE::BSTEventSink<RE::MenuOpenCloseEvent>
    {
        EventRes ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

        void RegisterMenuEvents()
        {
            if (const auto scripts = RE::UI::GetSingleton()) {
                scripts->AddEventSink<RE::MenuOpenCloseEvent>(this);
                REX::INFO("Registered {}", typeid(RE::MenuOpenCloseEvent).name());
            }
        }

        inline void CloseMenu(RE::BSFixedString a_menuName)
        {
            if (const auto UIMsgQueue = RE::UIMessageQueue::GetSingleton(); UIMsgQueue) {
                UIMsgQueue->AddMessage(a_menuName, RE::UI_MESSAGE_TYPE::kHide, nullptr);
            }
        }
    };
} // namespace Events
