#pragma once
#include "mod-data.h"

using namespace MOD;
namespace Forms {
	struct Loader : public REX::Singleton<Loader> {

        inline static RE::BGSExplosion* spawn_visual_explosion{ nullptr };
        inline static RE::BGSExplosion* spawn_urn_explosion{ nullptr };
        inline static RE::TESNPC* npc_spawn_generic{ nullptr };
        inline static RE::TESNPC* container_spawn_draugr{ nullptr };
        inline static RE::TESNPC* container_spawn_dwarven{ nullptr };
        inline static RE::TESNPC* container_spawn_warlock{ nullptr };
        inline static RE::TESNPC* npc_spawn_werewolf{ nullptr };
        inline static RE::TESNPC* container_spawn_mimic{ nullptr };
        inline static RE::TESFaction* werewolf_faction{ nullptr };
        inline static RE::SpellItem* stress_spell{ nullptr };
        inline static RE::BGSSoundDescriptorForm* meme_sound{ nullptr };

        void LogForm(RE::TESForm* a_form) {
            REX::INFO("Form {} is the ID of {}", typeid(a_form).name(), a_form->GetFormID());
        }

        inline void LoadForms(bool with_logging) {
            using namespace RE;
            const auto dh = RE::TESDataHandler::GetSingleton();

            if (const auto file = dh->LookupModByName(MIMIC_MOD_NAME); !file || file->compileIndex == 0xFF) {
                REX::FAIL(std::format("Can not load {} please make sure the mod is enabled!", MIMIC_MOD_NAME.data()));
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

            if (with_logging) {
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
            }
        }

	};
}
