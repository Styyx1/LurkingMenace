#pragma once

namespace MOD
{
// config loading
inline constexpr std::string_view TOML_PATH_DEFAULT = "Data/SKSE/Plugins/lurkingmenace.toml";
inline constexpr std::string_view TOML_PATH_CUSTOM = "Data/SKSE/Plugins/lurkingmenace_custom.toml";
inline constexpr std::string_view JSON_PATH = "Data/SKSE/Plugins/MimicExceptions";
// section names
inline constexpr std::string_view SECTION_SETTINGS = "Settings";
inline constexpr std::string_view SECTION_TOGGLES = "Toggles";
inline constexpr std::string_view SECTION_DEBUG = "Debugging";
inline constexpr std::string_view SECTION_TIMERS = "Timers";

// form loading
inline constexpr std::string_view MIMIC_MOD_NAME = "SurpriseSpawn.esp";
inline constexpr std::string_view SKYRIM_MOD_NAME = "Skyrim.esm";
inline constexpr i32 URN_EXPLOSION_SPAWN = 0x801;
inline constexpr i32 VISUAL_EXPLOSIONS_F_SPAWNS = 0x808;
inline constexpr i32 CHEST_MIMIC_SPAWN = 0x814;
inline constexpr i32 CHEST_SPAWNS_DRAUGR = 0x80a;
inline constexpr i32 CHEST_SPAWNS_DWARVEN = 0x80e;
inline constexpr i32 CHEST_SPAWNS_WARLOCK = 0x80f;
inline constexpr i32 CORPSE_GENERIC_NPC_SPAWN = 0x800;
inline constexpr i32 CORPSE_SPAWN_WEREWOLF = 0x811;
inline constexpr i32 MEME_SOUND = 0x810;
inline constexpr i32 STRESS_SPELL = 0x816;
inline constexpr i32 WEREWOLF_FACTION = 0x43594;
// formlists
inline constexpr i32 NPC_SPAWN_LIST_GENERIC = 0x818;
inline constexpr i32 NPC_SPAWN_LIST_WEREWOLF = 0x81A;
inline constexpr i32 NPC_SPAWN_LIST_VAMPIRE = 0x81B;
inline constexpr i32 NPC_SPAWN_LIST_DWARVEN = 0x81C;
inline constexpr i32 NPC_SPAWN_LIST_UNDEAD = 0x81D;
inline constexpr i32 NPC_SPAWN_LIST_DRAGON = 0x81E;
inline constexpr i32 CONTAINER_SPAWN_LIST_GENERIC = 0x81F;
inline constexpr i32 CONTAINER_SPAWN_LIST_DRAUGR = 0x820;
inline constexpr i32 CONTAINER_SPAWN_LIST_DWARVEN = 0x821;
inline constexpr i32 CONTAINER_SPAWN_LIST_WARLOCK = 0x822;

// misc
inline constexpr std::string_view ORE_KEYWORD = "ActiTypeOre";

} // namespace MOD
