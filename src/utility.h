#pragma once
#include "cache.h"
#include "settings.h"
#include "formloader.h"

struct Utility
{
    inline static bool DoesNameContain(const std::string& base_name, const std::string& to_search) {
        if (base_name.empty() || to_search.empty())
            return false;

        std::string lowercase_base = base_name;
        std::ranges::transform(lowercase_base, lowercase_base.begin(), ::tolower);
        std::string lowercase_search = to_search;
        std::ranges::transform(lowercase_search, lowercase_search.begin(), ::tolower);

        if (lowercase_base.contains(lowercase_search))
            return true;
        return false;
    }

    inline static bool HasRestrictedName(RE::TESObjectREFR* form) {

        std::string res_name = form->GetName();
        REX::INFO("name is {}", form->GetName());
        if (res_name.empty()) {
            REX::INFO("no name found");
            return false;
        }

        std::string lowered = res_name;
        std::ranges::transform(lowered, lowered.begin(), ::tolower);

        if (std::count(Config::JSONLoader::exception_names.begin(), Config::JSONLoader::exception_names.end(), lowered)) {
            REX::INFO("{} is a restricted name", res_name);
            return true;
        }
        else
            return false;

    }

    static inline bool IsRestrictedForm(RE::TESForm* form) {
        if (Config::JSONLoader::exception_forms.contains(form))
            return true;
        return false;
    }

    inline static bool isRestrictedCell()
    {
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
        auto                     cell = ActorUtil::GetPlayerCell(player);

        if (!cell)
            return false;

        if (Config::JSONLoader::exception_cells.contains(cell)) {
            REX::INFO("{} is a restricted Cell", player->GetParentCell()->GetFormEditorID());
            return true;
        }
        else {
            return false;
        }
    };

    inline static bool isRestrictedLoc()
    {
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();

        if (player->GetCurrentLocation()) {
            REX::INFO("location is {}", player->GetParentCell()->GetFormEditorID());
            return player->GetCurrentLocation()->HasAnyKeywordByEditorID(Config::JSONLoader::exception_keywords);
        }
        else {
            REX::INFO("no location found");
            return false;
        }
    }

    inline static void LogOwnership(RE::TESObjectREFR* obj) {
        if (obj->GetActorOwner()) {
            REX::INFO("owner of {} is {}", obj->GetName(), obj->GetActorOwner()->GetName());
            return;
        }
        else {
            REX::INFO("object {} has no owner actor", obj->GetName());
            return;
        }

    }

    inline static bool isAnyException(RE::TESObjectREFR* form)
    {
        if (IsRestrictedForm(form)) {
            REX::INFO("Is restricted form");
            return true;
        }
        if (isRestrictedCell()) {
            REX::INFO("restricted cell");
            return true;
        }
        if (isRestrictedLoc()) {
            REX::INFO("restricted location type keyword");
            return true;
        }
        if (HasRestrictedName(form)) {
            REX::INFO("Has restricted name");
            return true;
        }
        else
            return false;
    }

    inline static std::chrono::duration<double> GetTimer()
    {
        using set = Config::Settings;
        if (set::delay_time_range_active.GetValue()) {
            auto delay = RandomiserUtil::GetRandomDouble(set::delay_time_min.GetValue(), set::delay_time_max.GetValue());
            REX::INFO("random time delay is {}", delay);
            set::thread_delay = std::chrono::duration<double>(delay);
            return set::thread_delay;
        }
        else {
            set::thread_delay = std::chrono::duration<double>(set::delay_timer_seconds.GetValue());
            return set::thread_delay;
        }
    }

    

    


    /*bool isRestrictedContainer(std::string a_contEDID)
    {
        auto                     settings   = Settings::GetSingleton();
        std::vector<std::string> exceptions

     * * =
     * settings->JSONSettings["ContainerIDs"];        

        if (std::count(exceptions.begin(), exceptions.end(), a_contEDID.c_str())) {
 REX::INFO("{}
     * is
     * a
     * restricted container", a_contEDID);
            return true;
        }
        else {
            REX::INFO("{} is not restricted", a_contEDID);
 return

     *
     * * false;
        }
    }

    bool isRestrictedNameOrID(std::string a_name, std::string a_contEDID)
    {
        if (ExceptionName(a_name)) {
            return true;
 }


     * * if
     * (isRestrictedContainer(a_contEDID)) {
            return true;
        }
        else {
            return false;
        }
    }*/

    

    inline static void RemoveAllItems(RE::TESObjectREFR* a_refToRemoveFrom, RE::TESObjectREFR* a_refToGiveItems)
    {
        auto inv_map = a_refToRemoveFrom->GetHandle().get()->GetInventoryCounts();
        for (auto& items : inv_map) {
            if (items.first->GetFormType() != RE::FormType::LeveledItem) {
                a_refToRemoveFrom->GetHandle().get()->RemoveItem(items.first, items.second, RE::ITEM_REMOVE_REASON::kRemove, nullptr, a_refToGiveItems);
                REX::INFO("removed {}", items.first->GetName());
            }
            else
                return;
        }
    }

    inline static void PlayMeme()
    {
        RE::PlayerCharacter* p  = Cache::GetPlayerSingleton();
        if (Config::Settings::meme_sound_active.GetValue()) {
            SoundUtil::PlaySound(p, Forms::Loader::meme_sound, 1.5f);
        }
        else
            return;
    }

    inline static bool LocationCheck(std::string_view locKeyword)
    {
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();

        if (player->GetCurrentLocation() != nullptr) {
            return player->GetCurrentLocation()->HasKeywordString(locKeyword);
            // REX::INFO("current location is: {}", player->GetCurrentLocation()->GetName());
        }
        else {
            REX::INFO("no location found");
            return false;
        }
    }

    inline static bool LocPlayerOwned()
    {
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();

        if (player->GetCurrentLocation() != nullptr) {
            if (player->GetCurrentLocation()->HasKeywordString("LocTypePlayerHouse")) {
                // REX::INFO("current location is: {}", player->GetCurrentLocation()->GetName());
                return true;
            }
            else
                return false;
        }
        else {
            // REX::INFO("current location is: {}", player->GetCurrentLocation()->GetName());
            return false;
        }
    }

    inline static void ApplyStress(RE::Actor* target)
    {
        RE::PlayerCharacter* player   = Cache::GetPlayerSingleton();
        MagicUtil::ApplySpell(player, target, Forms::Loader::stress_spell);
        REX::INFO("applied {} to {}", Forms::Loader::stress_spell->GetName(), target->AsReference()->GetName());
    }

    enum class SpawnEvent : uint32_t {
        kDraugr = 1,
        kDwarven = 2,
        kWarlock = 3,
        kUrn = 4,
        kGeneric = 5,
        kNone = 0,
    };
    inline static SpawnEvent GetSpawnEvent(RE::TESForm* reference) {
        using s = Config::Settings;
        if (s::container_spawn_draugr_active.GetValue() && DoesNameContain(reference->GetName(), "draugr")) {
            return SpawnEvent::kDraugr;
        }
        if (s::container_spawn_dwarven_active.GetValue() && LocationCheck("LocTypeDwarvenAutomatons")) {
            return SpawnEvent::kDwarven;
        }
        if (s::container_spawn_warlock_active.GetValue() && (LocationCheck("LocTypeWarlockLair") || LocationCheck("LocTypeVampireLair"))) {
            return SpawnEvent::kWarlock;
        }
        RE::PlayerCharacter* const& player = RE::PlayerCharacter::GetSingleton();
        if (s::explosion_spawn_urn.GetValue() && DoesNameContain(reference->GetName(), "urn") && CellUtil::IsDungeon(ActorUtil::GetPlayerCell(player))) {
            return SpawnEvent::kUrn;
        }
        if (s::container_spawn_mimic_active.GetValue()) {
            return SpawnEvent::kGeneric;
        }

        return SpawnEvent::kNone;

    }
};
