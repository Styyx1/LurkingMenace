#include "settings.h"
#include "utility.h"

void Settings::LoadSettings()
{
    CSimpleIniA ini;
    logger::info("Loading settings");

    constexpr auto defaultSettingsPath = L"Data/MCM/Config/SurpriseSpawn/settings.ini";
    constexpr auto mcmPath = L"Data/MCM/Settings/SurpriseSpawn.ini";

    UpdateSettings(ini, defaultSettingsPath);
    UpdateSettings(ini, mcmPath);

    EnableDebugLogging();

    if (compareValue > maxNumber || compareValue < minNumber) {
        compareValue = maxNumber;
        logger::debug("adjusted compare value to min and max");
    }

    LoadExceptionJSON(L"Data/SKSE/Plugins/MimicExceptions.json");
    logger::info("Loaded settings");
};

double Settings::GetRandomDouble(double a_min, double a_max)
{
    static std::random_device        rd;
    static std::mt19937              gen(rd());
    std::uniform_real_distribution<> distrib(a_min, a_max);
    logger::debug("random double is {}", distrib(gen));
    return distrib(gen);
}

void Settings::LoadExceptionJSON(const wchar_t* a_path)
{
    std::ifstream i(a_path);
    i >> JSONSettings;
    logger::debug("Loaded Json");
}

void Settings::EnableDebugLogging() {
    if (debug_logging == true) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    }
    else 
        logger::info("debug logging disabled");
}

void Settings::LogForm(RE::TESForm* a_form) {
    logger::info("Form {} is the ID of {}", typeid(a_form).name(), a_form->GetFormID());
}
// "Data/MCM/Settings/SurpriseSpawn.ini"
void Settings::UpdateSettings(CSimpleIniA &ini , std::filesystem::path path)
{     
    ini.SetUnicode();
    ini.LoadFile(path.string().c_str());
    auto file = path.string();
    logger::info("loading ini file from {}",file);
    maxNumber    = std::stoi(ini.GetValue("General", "iMaxNumberRand", "40")); // MCM
    minNumber    = std::stoi(ini.GetValue("General", "iMinNumberRand", "1")); // MCM
    compareValue = std::stoi(ini.GetValue("General", "iCompareValue", "18")); // MCM
    npc_event_active               = ini.GetBoolValue("Event Toggles", "bNPCEvent", true); // MCM
    draugr_container_event_active  = ini.GetBoolValue("Event Toggles", "bDraugrContainerEvent", true); // MCM
    dwarven_container_event_active = ini.GetBoolValue("Event Toggles", "bDwarvenContainerEvent", true); // MCM
    shade_container_event_active   = ini.GetBoolValue("Event Toggles", "bShadeContainerEvent", true); // MCM
    generic_container_event_active = ini.GetBoolValue("Event Toggles", "bGenericContainerEvents", true); // MCM
    urn_explosion_event_active     = ini.GetBoolValue("Event Toggles", "bUrnContainerEvents", true); // MCM
    toggle_visual_explosion        = ini.GetBoolValue("Event Toggles", "bToggleExplosionVisuals", true); // MCM
    delayed_explosion              = ini.GetBoolValue("General", "bDelayedExplosion", false); // MCM
    useDelayRange                  = ini.GetBoolValue("General", "bUseDelayRange", true); // MCM
    delay_timer = ini.GetDoubleValue("General", "fDelayTimer", 2.5); // MCM
    maxTime     = ini.GetDoubleValue("General", "iMaxDelayTime", 2.5); // MCM
    minTime     = ini.GetDoubleValue("General", "iMinDelayTime", 12.0); // MCM
    toggle_meme_sound = ini.GetBoolValue("Fun", "bMemeSound", false); // MCM
    debug_logging     = ini.GetBoolValue("Debugging", "bEnableDebug"); // MCM
}

void Settings::LoadForms() noexcept
{
    auto dataHandler = RE::TESDataHandler::GetSingleton();

    const int gen_npc_spawn = 0x800;
    const int spawn_urn_explosion = 0x801;
    const int spawn_explosions = 0x808;
    const int draugr_chest_spawn = 0x80A;
    const int dwarven_chest_spawn = 0x80E;
    const int shade_spawn = 0x80F;
    const int werewolf_enemy_spawn = 0x811;
    const int mimic_chest_spawn = 0x814;
    const int meme_sound_id = 0x810;
    const int stress_spell_id = 0x816;

    logger::info("Loading forms");
    const char* mName = "SurpriseSpawn.esp";
    //Hard coded cause it makes no sense having all the required records in a separate esp, at this point, just change my esp
    SpawnEnemy = skyrim_cast<RE::TESNPC*>(dataHandler->LookupForm(gen_npc_spawn, mName));        
    SpawnExplosion = skyrim_cast<RE::BGSExplosion*>(dataHandler->LookupForm(spawn_explosions, mName));        
    UrnExplosion = skyrim_cast<RE::BGSExplosion*>(dataHandler->LookupForm(spawn_urn_explosion, mName));        
    DraugrEnemy = skyrim_cast<RE::TESNPC*>(dataHandler->LookupForm(draugr_chest_spawn, mName));        
    DwarvenEnemy = skyrim_cast<RE::TESNPC*>(dataHandler->LookupForm(dwarven_chest_spawn, mName));        
    ShadeEnemy = skyrim_cast<RE::TESNPC*>(dataHandler->LookupForm(shade_spawn, mName));        
    MemeSound = skyrim_cast<RE::BGSSoundDescriptorForm*>(dataHandler->LookupForm(meme_sound_id, mName));        
    WerewolfEnemy = skyrim_cast<RE::TESNPC*>(dataHandler->LookupForm(werewolf_enemy_spawn, mName));        
    MimicEnemy = skyrim_cast<RE::TESNPC*>(dataHandler->LookupForm(mimic_chest_spawn, mName));        
    StressSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(stress_spell_id, mName));  

    if (debug_logging) {
        LogForm(SpawnEnemy);
        LogForm(SpawnExplosion);
        LogForm(UrnExplosion);
        LogForm(DraugrEnemy);
        LogForm(DwarvenEnemy);
        LogForm(ShadeEnemy);
        LogForm(MemeSound);
        LogForm(WerewolfEnemy);
        LogForm(MimicEnemy);
        LogForm(StressSpell);
    }

    WerewolfFaction = dataHandler->LookupForm(0x43594, "Skyrim.esm")->As<RE::TESFaction>();
    logger::debug("loaded Faction: {}", WerewolfFaction->GetName());

    logger::info("All Forms loaded");

} // Credits: Colinswrath https://github.com/colinswrath/BladeAndBlunt
