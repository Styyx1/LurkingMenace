#pragma once

#include <API/SKSEMenuFramework.h>
#include "mod-data.h"
#include "settings.h"

namespace UI {
	using str = std::string;
	bool Register();

	void RestoreSettingsFromConfig(bool settings, bool toggles);
	void RestoreDefaultSettings(bool settings, bool toggles);
	void HelpMarker(const char* desc);

	namespace MenuTitles {
		inline str ModName = "Lurking Menace";
		inline str Settings = "Settings";
		inline str Toggles = "Spawn Toggles";
		inline str Save = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
		inline str Restore = FontAwesome::UnicodeToUtf8(0xf0e2) + " Restore Default Settings";
		inline str System = FontAwesome::UnicodeToUtf8(0xf390) + " System";
		inline str Timers = FontAwesome::UnicodeToUtf8(0xf017) + " Timers";
		inline str Spawns = FontAwesome::UnicodeToUtf8(0xf6e2) + " Spawns";
		inline str Misc = FontAwesome::UnicodeToUtf8(0xf67b) + " Misc Toggles";
	}
	namespace Settings {

		void __stdcall RenderSettings();

		namespace Label {
			inline str spawn_chance = "Spawn Chance";
			inline str use_delay_explosion_toggle = "Use Delayed Explosion";
			inline str use_delay_time_ranges = "Use Time Ranges";
			inline str delay_time_without_range = "Default Delay Time";
			inline str delay_time_max = "Maximum Delay";
			inline str delay_time_min = "Minimum Delay";
			inline str spawn_from_formlist = "Spawn from list";

		}
		namespace Tooltip {
			inline str spawn_chance = "Chance to spawn an enemy upon activating something";
			inline str use_delay_explosion_toggle = "Delay the explosion until something spawns or show it as soon as an event will happen";
			inline str use_delay_time_ranges = "Makes the spawn delay time have some randomness to it";
			inline str delay_time_without_range = "Default delay without any ranges. If you activate ranges, this setting will be ignored!";
			inline str delay_time_max = "Maximum delay for a spawn";
			inline str delay_time_min = "Minimum delay for a spawn";
			inline str spawn_from_formlist = "Toggle spawning from the formlists instead of the static single encounters";
		}

		inline float spawn_chance_temp;
		inline bool use_delayed_explosion_temp;
		inline bool use_time_ranges_temp;
		inline float delay_time_without_delay_temp;
		inline float delay_time_range_minimum_temp;
		inline float delay_time_range_maximum_temp;
		inline bool spawn_from_formlist;

	}
	namespace Toggles {

		void __stdcall RenderToggles();
		namespace Label {
			inline str toggle_npc_spawns = "Toggle Generic NPC Spawns";
			inline str toggle_werewolf_spawns = "Toggle Werewolf NPC Spawns";
			inline str toggle_draugr_spawns = "Toggle Draugr Spawns";
			inline str toggle_dwarven_spawns = "Toggle Dwarven Spawns";
			inline str toggle_warlock_spawns = "Toggle Warlock Spawns";
			inline str toggle_mimic_spawns = "Toggle Mimic Spawns";
			inline str toggle_urn_spawns = "Toggle Urn Explosions";
			inline str toggle_meme_sound = "Toggle Meme Sound";
			inline str toggle_explosion_visuals = "Toggle Explosion Visuals";
			inline str toggle_npc_vampires = "Toggle Vampire Spawns";
			inline str npc_toggle_undead = "Toggle From Undead";
			inline str npc_toggle_dwarven = "Toggle From Dwarven NPC";
			inline str npc_toggle_dragon = "Toggle From Dragons";
		}
		namespace Tooltip {
			inline str toggle_npc_spawns = "Toggle Spawns from Generic NPCs";
			inline str toggle_werewolf_spawns = "Toggle Spawns from dead werewolves";
			inline str toggle_draugr_spawns = "Toggle spawns from CONTAINERS called draugr... \nThis is only affecting the static draugr in the game, not the enemies";
			inline str toggle_dwarven_spawns = "Toggle dwarven spawns inside dwemer ruins";
			inline str toggle_warlock_spawns = "Toggle specific spawns inside warlock locations or vampire lairs";
			inline str toggle_mimic_spawns = "Toggle general mimics for anything not further specialised like the other settings";
			inline str toggle_urn_spawns = "Toggle explosions that can happen by opening Urns";
			inline str toggle_meme_sound = "Toggle Meme Sound";
			inline str toggle_explosion_visuals = "Toggle Explosion Visuals";
			inline str toggle_npc_vampires = "Toggle Spawns from dead vampires";
			inline str npc_toggle_undead = "Toggle Spawns from dead undead NPCs";
			inline str npc_toggle_dwarven = "Toggle Spawns from dwarven NPCs";
			inline str npc_toggle_dragon = "Toggle Spawns from dragons";
		}

		inline bool toggle_npc_spawns_temp;
		inline bool toggle_werewolf_spawns_temp;
		inline bool toggle_draugr_temp;
		inline bool toggle_dwarven_temp;
		inline bool toggle_warlock_temp;
		inline bool toggle_mimic_temp;
		inline bool toggle_urn_temp;
		inline bool toggle_meme_sound_temp;
		inline bool toggle_explosion_visuals_temp;
		inline bool toggle_npc_vampire;
		inline bool npc_toggle_undead;
		inline bool npc_toggle_dwarven;
		inline bool npc_toggle_dragon;
	}
}