#pragma once
#include "mod-data.h"

using namespace MOD;
namespace Forms
{
struct Loader : public REX::Singleton<Loader>
{

	inline static RE::BGSExplosion *spawn_visual_explosion{ nullptr };
	inline static RE::BGSExplosion *spawn_urn_explosion{ nullptr };
	inline static RE::TESNPC *npc_spawn_generic{ nullptr };
	inline static RE::TESNPC *container_spawn_draugr{ nullptr };
	inline static RE::TESNPC *container_spawn_dwarven{ nullptr };
	inline static RE::TESNPC *container_spawn_warlock{ nullptr };
	inline static RE::TESNPC *npc_spawn_werewolf{ nullptr };
	inline static RE::TESNPC *container_spawn_mimic{ nullptr };
	inline static RE::TESFaction *werewolf_faction{ nullptr };
	inline static RE::SpellItem *stress_spell{ nullptr };
	inline static RE::BGSSoundDescriptorForm *meme_sound{ nullptr };

	inline static RE::BGSListForm *npc_spawn_formlist_generic{ nullptr };
	inline static RE::BGSListForm *npc_spawn_formlist_werewolf{ nullptr };
	inline static RE::BGSListForm *npc_spawn_formlist_vampire{ nullptr };
	inline static RE::BGSListForm *npc_spawn_formlist_dwarven{ nullptr };
	inline static RE::BGSListForm *npc_spawn_formlist_undead{ nullptr };
	inline static RE::BGSListForm *npc_spawn_formlist_dragon{ nullptr };

	inline static RE::BGSListForm *cont_spawn_formlist_generic{ nullptr };
	inline static RE::BGSListForm *cont_spawn_formlist_draugr{ nullptr };
	inline static RE::BGSListForm *cont_spawn_formlist_dwarven{ nullptr };
	inline static RE::BGSListForm *cont_spawn_formlist_warlock{ nullptr };

	inline static RE::BGSListForm *ore_tool_formlist{ nullptr };

	using VECN = std::vector<RE::TESNPC *>;
	inline static VECN npc_spawn_generic_vec;
	inline static VECN npc_spawn_vec_werefolf;
	inline static VECN npc_spawn_vec_vampire;
	inline static VECN npc_spawn_vec_dwarven;
	inline static VECN npc_spawn_vec_undead;
	inline static VECN npc_spawn_vec_dragon;
	inline static VECN cont_spawn_vec_generic;
	inline static VECN cont_spawn_vec_draugr;
	inline static VECN cont_spawn_vec_dwarven;
	inline static VECN cont_spawn_vec_warlock;

	inline static std::unordered_set<RE::TESObjectWEAP*> ore_tool_list;

	inline void FormlistToVector( RE::BGSListForm *list, std::vector<RE::TESNPC *> &vec )
	{
		if ( list )
		{
			list->ForEachForm( [&]( RE::TESForm *a_formInList )
			{
				auto npc = a_formInList->As<RE::TESNPC>();
				if ( npc )
				{
					vec.emplace_back( npc );
					return RE::BSContainer::ForEachResult::kContinue;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			} );
		}
	}

	inline void FillToolVec( RE::BGSListForm *list, std::unordered_set<RE::TESObjectWEAP *> &set )
	{
		if ( !list )
			return;
		list->ForEachForm( [&]( RE::TESForm *a_formInList )
		{
			auto weap = a_formInList->As<RE::TESObjectWEAP>();
			if ( weap )
			{
				set.insert( weap );
				return RE::BSContainer::ForEachResult::kContinue;
			}

			return RE::BSContainer::ForEachResult::kContinue;
		} );       
	}


    void LogForm(RE::TESForm *a_form)
    {
        REX::DEBUG("Form {} is the ID of {}", typeid(a_form).name(), a_form->GetFormID());
    }
    void DebugVecSize(std::string name, std::vector<RE::TESNPC *> &vec)
    {
        REX::DEBUG("vector {} size is: {}", name, vec.size());
    }

    inline void LoadForms(bool with_logging)
    {
        using namespace RE;
        const auto dh = RE::TESDataHandler::GetSingleton();

        if (const auto file = dh->LookupModByName(MIMIC_MOD_NAME); !file || file->compileIndex == 0xFF)
        {
			std::string mod_name = MIMIC_MOD_NAME.data();
			REX::FAIL( std :: format( "Can not load {} please make sure the mod is enabled!", mod_name) );
        }

        spawn_visual_explosion = dh->LookupForm<BGSExplosion>(VISUAL_EXPLOSIONS_F_SPAWNS, MIMIC_MOD_NAME);
        spawn_urn_explosion = dh->LookupForm<BGSExplosion>(URN_EXPLOSION_SPAWN, MIMIC_MOD_NAME);
        npc_spawn_generic = dh->LookupForm<TESNPC>(CORPSE_GENERIC_NPC_SPAWN, MIMIC_MOD_NAME);
        npc_spawn_werewolf = dh->LookupForm<TESNPC>(CORPSE_SPAWN_WEREWOLF, MIMIC_MOD_NAME);
        container_spawn_draugr = dh->LookupForm<TESNPC>(CHEST_SPAWNS_DRAUGR, MIMIC_MOD_NAME);
        container_spawn_dwarven = dh->LookupForm<TESNPC>(CHEST_SPAWNS_DWARVEN, MIMIC_MOD_NAME);
        container_spawn_warlock = dh->LookupForm<TESNPC>(CHEST_SPAWNS_WARLOCK, MIMIC_MOD_NAME);
        container_spawn_mimic = dh->LookupForm<TESNPC>(CHEST_MIMIC_SPAWN, MIMIC_MOD_NAME);
        stress_spell = dh->LookupForm<SpellItem>(STRESS_SPELL, MIMIC_MOD_NAME);
        meme_sound = dh->LookupForm<BGSSoundDescriptorForm>(MEME_SOUND, MIMIC_MOD_NAME);
        werewolf_faction = dh->LookupForm<TESFaction>(WEREWOLF_FACTION, SKYRIM_MOD_NAME);
        /// NPC Spawns
        npc_spawn_formlist_generic = dh->LookupForm<BGSListForm>(NPC_SPAWN_LIST_GENERIC, MIMIC_MOD_NAME);
        npc_spawn_formlist_werewolf = dh->LookupForm<BGSListForm>(NPC_SPAWN_LIST_WEREWOLF, MIMIC_MOD_NAME);
        npc_spawn_formlist_vampire = dh->LookupForm<BGSListForm>(NPC_SPAWN_LIST_VAMPIRE, MIMIC_MOD_NAME);
        npc_spawn_formlist_dwarven = dh->LookupForm<BGSListForm>(NPC_SPAWN_LIST_DWARVEN, MIMIC_MOD_NAME);
        npc_spawn_formlist_undead = dh->LookupForm<BGSListForm>(NPC_SPAWN_LIST_UNDEAD, MIMIC_MOD_NAME);
        npc_spawn_formlist_dragon = dh->LookupForm<BGSListForm>(NPC_SPAWN_LIST_DRAGON, MIMIC_MOD_NAME);
        /// Container Spawns
        cont_spawn_formlist_generic = dh->LookupForm<BGSListForm>(CONTAINER_SPAWN_LIST_GENERIC, MIMIC_MOD_NAME);
        cont_spawn_formlist_draugr = dh->LookupForm<BGSListForm>(CONTAINER_SPAWN_LIST_DRAUGR, MIMIC_MOD_NAME);
        cont_spawn_formlist_dwarven = dh->LookupForm<BGSListForm>(CONTAINER_SPAWN_LIST_DWARVEN, MIMIC_MOD_NAME);
        cont_spawn_formlist_warlock = dh->LookupForm<BGSListForm>(CONTAINER_SPAWN_LIST_WARLOCK, MIMIC_MOD_NAME);

        FormlistToVector(npc_spawn_formlist_generic, npc_spawn_generic_vec);
        DebugVecSize("npc_spawn_generic_vec", npc_spawn_generic_vec);

        FormlistToVector(npc_spawn_formlist_werewolf, npc_spawn_vec_werefolf);
        DebugVecSize("npc_spawn_vec_werefolf", npc_spawn_vec_werefolf);

        FormlistToVector(npc_spawn_formlist_vampire, npc_spawn_vec_vampire);
        DebugVecSize("npc_spawn_vec_vampire", npc_spawn_vec_vampire);

        FormlistToVector(npc_spawn_formlist_dwarven, npc_spawn_vec_dwarven);
        DebugVecSize(std::string("npc_spawn_vec_dwarven"), npc_spawn_vec_dwarven);

        FormlistToVector(npc_spawn_formlist_undead, npc_spawn_vec_undead);
        DebugVecSize(std::string("npc_spawn_vec_undead"), npc_spawn_vec_undead);

        FormlistToVector(npc_spawn_formlist_dragon, npc_spawn_vec_dragon);
        DebugVecSize(std::string("npc_spawn_vec_dragon"), npc_spawn_vec_dragon);

        FormlistToVector(cont_spawn_formlist_generic, cont_spawn_vec_generic);
        DebugVecSize(std::string("cont_spawn_vec_generic"), cont_spawn_vec_generic);

        FormlistToVector(cont_spawn_formlist_draugr, cont_spawn_vec_draugr);
        DebugVecSize(std::string("cont_spawn_vec_draugr"), cont_spawn_vec_draugr);

        FormlistToVector(cont_spawn_formlist_dwarven, cont_spawn_vec_dwarven);
        DebugVecSize(std::string("cont_spawn_vec_dwarven"), cont_spawn_vec_dwarven);

        FormlistToVector(cont_spawn_formlist_warlock, cont_spawn_vec_warlock);
        DebugVecSize(std::string("cont_spawn_vec_warlock"), cont_spawn_vec_warlock);

        FillToolVec( ore_tool_formlist, ore_tool_list );

        if (with_logging)
        {
            LogForm(spawn_visual_explosion);
            LogForm(spawn_urn_explosion);
            LogForm(npc_spawn_generic);
            LogForm(npc_spawn_werewolf);
            LogForm(container_spawn_draugr);
            LogForm(container_spawn_dwarven);
            LogForm(container_spawn_warlock);
            LogForm(container_spawn_mimic);
            LogForm(stress_spell);
            LogForm(meme_sound);
            LogForm(werewolf_faction);
            LogForm(npc_spawn_formlist_generic);
        }
    }
};
} // namespace Forms
