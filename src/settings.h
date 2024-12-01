#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Settings : public Singleton<Settings>
{
public:
    void LoadSettings();
    void   LoadForms() noexcept;
    void   LoadExceptionJSON(const wchar_t* a_path);
    double GetRandomDouble(double a_min, double a_max);
    static void EnableDebugLogging();
    void LogForm(RE::TESForm* a_form);
    void UpdateSettings(CSimpleIniA &ini, std::filesystem::path path);

    json JSONSettings;

    int         minNumber{1};
    int         maxNumber{40};
    int         compareValue{18};
    // Forms
    RE::BGSExplosion* SpawnExplosion{};
    RE::BGSExplosion* UrnExplosion{};
    RE::TESNPC* SpawnEnemy{}; // had to be TESNPC* cause Actor* doesn't work
    RE::TESNPC* DraugrEnemy{};
    RE::TESNPC* DwarvenEnemy{};
    RE::TESNPC* ShadeEnemy{};
    RE::TESNPC* WerewolfEnemy{};
    RE::TESNPC* MimicEnemy{};
    RE::TESFaction* WerewolfFaction{};
    RE::SpellItem* StressSpell{};
    std::chrono::duration<double> thread_delay{};
    RE::BGSSoundDescriptorForm* MemeSound{};

    inline static bool   debug_logging{false};
    inline static bool   npc_event_active{ true };
    inline static bool   draugr_container_event_active{ true };
    inline static bool   dwarven_container_event_active{ true };
    inline static bool   shade_container_event_active{ true };
    inline static bool   generic_container_event_active{ true };
    inline static bool   urn_explosion_event_active{ true };
    inline static bool   toggle_meme_sound{ false };
    inline static bool   toggle_visual_explosion{ true };
    inline static bool   delayed_explosion{ false };
    inline static double delay_timer{ 2.5 };
    inline static bool   useDelayRange{ true };
    inline static double minTime;
    inline static double maxTime;
    inline static bool enable_human_npc_event{ true };
};
