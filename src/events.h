#pragma once
#include "settings.h"

namespace Events
{
    using EventRes = RE::BSEventNotifyControl;
    void RegisterEvents();

    struct RefTimer
	{
		RE::TESObjectREFR *a_ref;
		TimerUtil::Timer a_timer;
		std::unordered_map<RE::TESObjectREFR *, TimerUtil::Timer> timer_map;

        bool IsActive( RE::TESObjectREFR *ref ) const
		{
			if ( !ref )
				return false;
			auto it = timer_map.find( ref );
			return it != timer_map.end() && it->second.IsRunning();
		}

		void Start( RE::TESObjectREFR *ref )
		{
			if ( !ref )
				return;
			timer_map[ref].Start();
		}

		void StopIfExpired( RE::TESObjectREFR *ref, std::chrono::duration<double> duration )
		{
			if ( !ref )
				return;
			auto it = timer_map.find( ref );
			if ( it != timer_map.end() && it->second.ElapsedSeconds() >= duration.count() )
				it->second.Stop();
		}
    };

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
		inline static RefTimer ref_timer;
        inline static TimerUtil::Timer check_timer;
        void DelayedSpawn(RE::TESObjectREFR* source, RE::TESNPC* npc_to_spawn, RE::TESBoundObject* explosion, std::chrono::duration<double> delay,bool disable_source = false);
    };

    struct HitEvent : public REX::Singleton<HitEvent>, public RE::BSTEventSink<RE::TESHitEvent>
	{
		void Register() {
			if ( const auto src = RE::ScriptEventSourceHolder::GetSingleton() )
			{
				src->AddEventSink<RE::TESHitEvent>( this );
				REX::INFO( "Registered {}", typeid( RE::TESHitEvent ).name() );
            }
        };
		EventRes ProcessEvent( const RE::TESHitEvent *event, RE::BSTEventSource<RE::TESHitEvent> * ) override;


    };

    struct ContainerMenuHook
	{
	  private: 
        inline static RE::UI_MESSAGE_RESULTS ProcessMessage( RE::ContainerMenu *a_this, RE::UIMessage &a_message );
		inline static REL::HookVFT _Hook0{ RE::VTABLE_ContainerMenu[0], 0x04, ProcessMessage };
    };
	} // namespace Events
