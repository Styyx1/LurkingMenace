#pragma once
#include <nlohmann/json.hpp>
#include "mod-data.h"
using json = nlohmann::json;
using namespace MOD;
namespace Config {
    struct Settings : public REX::Singleton<Settings> {
        static inline REX::TOML::Bool debug_logging{ SECTION_DEBUG, "bEnableDebug", false };

        static inline REX::TOML::F32 mimic_chance{ SECTION_SETTINGS, "fEventSpawnChance", 50.0f };

        static inline REX::TOML::Bool spawn_from_npcs{ SECTION_TOGGLES, "bSpawnFromNPCs", true };
        static inline REX::TOML::Bool container_spawn_draugr_active{ SECTION_TOGGLES, "bChestSpawnDraugr", true };
        static inline REX::TOML::Bool container_spawn_dwarven_active{ SECTION_TOGGLES, "bChestSpawnDwarven", true };
        static inline REX::TOML::Bool container_spawn_warlock_active{ SECTION_TOGGLES, "bChestSpawnWarlock", true };
        static inline REX::TOML::Bool container_spawn_mimic_active{ SECTION_TOGGLES, "bChestSpawnMimic", true };
        static inline REX::TOML::Bool explosion_spawn_urn{ SECTION_TOGGLES, "bUrnExplosions", true };
        static inline REX::TOML::Bool meme_sound_active{ SECTION_TOGGLES, "bUseMemeSound", false };
        static inline REX::TOML::Bool visual_explosions_for_spawns_active{ SECTION_TOGGLES, "bUseVisualExplosions", true };

        static inline REX::TOML::Bool delay_explosion_active{ SECTION_TIMERS, "bUseDelayedExplosion", true };
        static inline REX::TOML::Bool delay_time_range_active{ SECTION_TIMERS, "bUseDelayTimeRanges", true };
        static inline REX::TOML::F64 delay_timer_seconds{ SECTION_TIMERS, "fDelayTime", 2.5 };
        static inline REX::TOML::F64 delay_time_min{ SECTION_TIMERS, "fDelayTimeMinimum", 1.0 };
        static inline REX::TOML::F64 delay_time_max{ SECTION_TIMERS, "fDelayTimeMaximum", 10.0 };

        static inline std::chrono::duration<double> thread_delay;

        inline bool ContainerEventsActive() {

            const auto values = {
                container_spawn_draugr_active.GetValue(),
                container_spawn_dwarven_active.GetValue(),
                container_spawn_warlock_active.GetValue(),
                container_spawn_mimic_active.GetValue(),
                explosion_spawn_urn.GetValue()
            };
            return std::any_of(values.begin(), values.end(), [](bool v) { return v; });
        }

        inline void UpdateSettings(bool a_doSave = false) {
            const auto toml = REX::TOML::SettingStore::GetSingleton();
            toml->Init(TOML_PATH_DEFAULT.data(), TOML_PATH_CUSTOM.data());
            if (!a_doSave)
                toml->Load();
            else
                toml->Save();
        }
    };

    struct JSONLoader : public REX::Singleton<JSONLoader>
    {
        using json = nlohmann::json;

        static inline std::unordered_set<RE::TESForm*> exception_forms;
        static inline std::vector<std::string> exception_names;
        static inline std::unordered_set<RE::TESObjectCELL*> exception_cells;
        static inline std::vector<std::string> exception_keywords;

        static inline json LoadJSON(const std::filesystem::path& path)
        {
            std::ifstream file(path);
            if (!file.is_open()) {
                REX::ERROR("Could not open JSON file '{}'", path.string());
                return {};
            }

            try {
                json j;
                file >> j;
                return j;
            }
            catch (const std::exception& e) {
                REX::ERROR("Failed to parse '{}': {}", path.string(), e.what());
                return {};
            }
        }
        static inline void LoadExceptionsFromFolder()
        {
            std::filesystem::path folder(JSON_PATH);

            if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
                REX::WARN("JSON folder '{}' does not exist or is not a directory.", folder.string());
                return;
            }

            ClearAll();

            for (const auto& entry : std::filesystem::directory_iterator(folder)) {
                if (!entry.is_regular_file() || entry.path().extension() != ".json")
                    continue;

                auto& path = entry.path();
                auto j = LoadJSON(path);
                if (j.empty())
                    continue;

                REX::INFO("Parsing '{}'", path.filename().string());

                ParseNames(j);
                ParseLocationKeys(j);
                ParseCells(j);
                ParseForms(j);
            }

            REX::INFO("Loaded all JSON exceptions: {} names, {} location keys, {} cells, {} forms",
                exception_names.size(), exception_keywords.size(),
                exception_cells.size(), exception_forms.size());
        }

        static void ParseNames(const json& j)
        {
            if (!j.contains("Names") || !j["Names"].is_array())
                return;

            for (auto& n : j["Names"]) {
                if (!n.is_string())
                    continue;

                std::string s = n.get<std::string>();

                // Lowercase once during load
                std::ranges::transform(s, s.begin(), ::tolower);

                exception_names.emplace_back(std::move(s));
            }
            REX::INFO("Loaded {} lowercase name exceptions.", exception_names.size());
        }

        static void ParseLocationKeys(const json& j)
        {
            if (!j.contains("LocationKeys") || !j["LocationKeys"].is_array()) return;
            for (auto& k : j["LocationKeys"])
                if (k.is_string())
                    exception_keywords.emplace_back(k.get<std::string>());
        }

        static void ParseCells(const json& j)
        {
            if (!j.contains("Cells") || !j["Cells"].is_array()) return;
            for (auto& c : j["Cells"]) {
                if (!c.is_string())
                    continue;

                auto name = c.get<std::string>();
                if (auto cell = RE::TESForm::LookupByEditorID<RE::TESObjectCELL>(name))
                    exception_cells.insert(cell);
                else
                    REX::WARN("Invalid cell name '{}'", name);
            }
        }

        static void ParseForms(const json& j)
        {
            if (!j.contains("Forms") || !j["Forms"].is_array()) return;
            for (auto& f : j["Forms"]) {
                if (!f.is_string())
                    continue;

                auto formStr = f.get<std::string>();
                if (auto form = FormUtil::GetFormFromString(formStr))
                    exception_forms.insert(form);
                else
                    REX::WARN("Invalid form '{}'", formStr);
            }
        }

        static void ClearAll()
        {
            exception_forms.clear();
            exception_names.clear();
            exception_cells.clear();
            exception_keywords.clear();
        }
    };

}
