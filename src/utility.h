#pragma once
#include "cache.h"
#include "settings.h"
#include "formloader.h"

struct Utility
{
	//mainly used for draugr looking containers cause otherwise they aren't easy to check for afaik
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
		REX::DEBUG("name is {}", form->GetName());
		if (res_name.empty()) {
			REX::DEBUG("no name found");
			return false;
		}

		std::string lowered = res_name;
		std::ranges::transform(lowered, lowered.begin(), ::tolower);

		if (std::count(Config::JSONLoader::exception_names.begin(), Config::JSONLoader::exception_names.end(), lowered)) {
			REX::DEBUG("{} is a restricted name", res_name);
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
			REX::DEBUG(std::format("{} is a restricted Cell", player->GetParentCell()->GetFormEditorID()));
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
			REX::DEBUG(std::format("location is {}", player->GetParentCell()->GetFormEditorID()));
			return player->GetCurrentLocation()->HasAnyKeywordByEditorID(Config::JSONLoader::exception_keywords);
		}
		else {
			REX::DEBUG("no location found");
			return false;
		}
	}

	inline static void LogOwnership(RE::TESObjectREFR* obj) {
		if (obj->GetActorOwner()) {
			REX::DEBUG("owner of {} is {}", obj->GetName(), obj->GetActorOwner()->GetName());
			return;
		}
		else {
			REX::DEBUG("object {} has no owner actor", obj->GetName());
			return;
		}

	}

	inline static bool isAnyException(RE::TESObjectREFR* form)
	{
		if (IsRestrictedForm(form) || IsRestrictedForm(form->GetBaseObject())) {
			REX::DEBUG("Is restricted form");
			return true;
		}
		if (isRestrictedCell()) {
			REX::DEBUG("restricted cell");
			return true;
		}
		if (isRestrictedLoc()) {
			REX::DEBUG("restricted location type keyword");
			return true;
		}
		if (HasRestrictedName(form)) {
			REX::DEBUG("Has restricted name");
			return true;
		}
		else
			return false;
	}
	//Gets duration for threads
	inline static std::chrono::duration<double> GetTimer()
	{
		using set = Config::Settings;
		if (set::delay_time_range_active.GetValue()) {
			auto delay = RandomiserUtil::GetRandomDouble(set::delay_time_min.GetValue(), set::delay_time_max.GetValue());
			REX::DEBUG(std::format("random time delay is {}", delay));
			set::thread_delay = std::chrono::duration<double>(delay);
			return set::thread_delay;
		}
		else {
			set::thread_delay = std::chrono::duration<double>(set::delay_timer_seconds.GetValue());
			return set::thread_delay;
		}
	}
	//Moves all items from the container into the inventory of the spawn
	inline static void RemoveAllItems(RE::TESObjectREFR* a_refToRemoveFrom, RE::TESObjectREFR* a_refToGiveItems)
	{
		auto inv_map = a_refToRemoveFrom->GetHandle().get()->GetInventoryCounts();
		for (auto& items : inv_map) {
			if (items.first->GetFormType() != RE::FormType::LeveledItem) {
				a_refToRemoveFrom->GetHandle().get()->RemoveItem(items.first, items.second, RE::ITEM_REMOVE_REASON::kRemove, nullptr, a_refToGiveItems);
				REX::DEBUG(std::format("removed {}", items.first->GetName()));
			}
			else
				return;
		}
	}
	//Most Important function of the entire mod
	inline static void PlayMeme()
	{
		RE::PlayerCharacter* p = Cache::GetPlayerSingleton();
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
		}
		else {
			return false;
		}
	}
	//should work with modded homes unless they are badly made
	inline static bool LocPlayerOwned()
	{
		RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
		if (player->GetCurrentLocation() != nullptr) {
			if (player->GetCurrentLocation()->HasKeywordString("LocTypePlayerHouse")) {
				return true;
			}
			else
				return false;
		}
		else {
			return false;
		}
	}
	//applies dummy spell that gets the stress increase keyword thanks to KID
	inline static void ApplyStress(RE::Actor* target)
	{
		RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
		MagicUtil::ApplySpell(player, target, Forms::Loader::stress_spell);
		REX::DEBUG(std::format("applied {} to {}", Forms::Loader::stress_spell->GetName(), target->AsReference()->GetName()));
	}

	enum class SpawnEvent : uint32_t {
		kDraugr = 1,
		kDwarven = 2,
		kWarlock = 3,
		kUrn = 4,
		kGeneric = 5,
		kNPCGeneric = 6,
		kNPCWerewolf = 7,
		kNPCVampire = 8,
		kNPCUndead = 9,
		kNPCDwarven = 10,
		kNPCDragon = 11,
		kNone = 0,
	};


	inline static std::string_view SpawnEventToString(SpawnEvent ev) {
		switch (ev) {
		case SpawnEvent::kDraugr:
			return "CONTAINER Draugr";
		case SpawnEvent::kDwarven:
			return "CONTAINER Dwarven";
		case SpawnEvent::kWarlock:
			return "CONTAINER Warlock";
		case SpawnEvent::kUrn:
			return "CONTAINER Urn";
		case SpawnEvent::kGeneric:
			return "CONTAINER Generic";
		case SpawnEvent::kNPCGeneric:
			return "NPC Generic";
		case SpawnEvent::kNPCWerewolf:
			return "NPC Werewolf";
		case SpawnEvent::kNPCVampire:
			return "NPC Vampire";
		case SpawnEvent::kNPCUndead:
			return "NPC Undead";
		case SpawnEvent::kNPCDwarven:
			return "NPC Dwarven";
		case SpawnEvent::kNPCDragon:
			return "NPC Dragon";
		default:
			return "NONE";
		}
	}

	inline static bool IsSpawnTypeEnabled(SpawnEvent type)
	{
		using s = Config::Settings;
		switch (type) {
		case SpawnEvent::kDraugr:
			return s::container_spawn_draugr_active.GetValue();
		case SpawnEvent::kDwarven:
			return s::container_spawn_dwarven_active.GetValue();
		case SpawnEvent::kWarlock:
			return s::container_spawn_warlock_active.GetValue();
		case SpawnEvent::kUrn:
			return s::explosion_spawn_urn.GetValue();
		case SpawnEvent::kGeneric:
			return s::container_spawn_mimic_active.GetValue();
		case SpawnEvent::kNPCGeneric:
			return s::npc_spawn_generic.GetValue();
		case SpawnEvent::kNPCWerewolf:
			return s::npc_spawn_werewolf.GetValue();
		case SpawnEvent::kNPCVampire:
			return s::npc_spawn_vampire.GetValue();
		case SpawnEvent::kNPCUndead:
			return s::npc_spawn_undead.GetValue();
		case SpawnEvent::kNPCDwarven:
			return s::npc_spawn_dwarven.GetValue();
		case SpawnEvent::kNPCDragon:
			return s::npc_spawn_dragon.GetValue();
		default:
			return false;
		}
	}

	//needed TESObjectREFR as argument instead of TESForm, but the event used has TESObjectREFR anyway
	inline static SpawnEvent GetSpawnEvent(RE::TESObjectREFR* reference) {
		using s = Config::Settings;
		// containers
		if (reference->GetBaseObject()->Is(RE::FormType::Container)) {
			if (IsSpawnTypeEnabled(SpawnEvent::kDraugr) && DoesNameContain(reference->GetName(), "draugr")) {
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
		}

		//NPCs
		if (reference->Is(RE::FormType::ActorCharacter)) {
			auto actor = reference->As<RE::Actor>();
			if (actor) {
				if (Config::Settings::npc_spawn_werewolf.GetValue() && actor->IsInFaction(Forms::Loader::werewolf_faction)) {
					return SpawnEvent::kNPCWerewolf;
				}
				if (Config::Settings::npc_spawn_vampire.GetValue() && ActorUtil::IsVampire(actor)) {
					return SpawnEvent::kNPCVampire;
				}
				if (ActorUtil::IsUndead(actor)) {
					return SpawnEvent::kNPCUndead;
				}
				if (actor && actor->GetParentCell() && Utility::LocationCheck("LocTypeDwarvenAutomatons")) {
					return SpawnEvent::kNPCDwarven;
				}
				if (ActorUtil::IsDragon(actor)) {
					return SpawnEvent::kNPCDragon;
				}

				if (Config::Settings::npc_spawn_generic.GetValue() && !ActorUtil::IsDragon(actor)) {
					return SpawnEvent::kNPCGeneric;
				}
			}
		}
		return SpawnEvent::kNone;
	}

	inline static RE::TESNPC* GetRandomNPCFromList(std::vector<RE::TESNPC*>& vec, RE::TESNPC* a_fallback = Forms::Loader::npc_spawn_generic) {
		if (vec.empty())
			return a_fallback;
		if (!Config::Settings::spawn_from_formlist.GetValue())
			return a_fallback;

		int rng = RandomiserUtil::GetRandomInt(0, static_cast<int>(vec.size()) - 1);
		return vec[rng] ? vec[rng] : a_fallback;
	}

	//takes forms from formloader. Will look for a way to make that more dynamic at some point
	inline static RE::TESNPC* GetNPCFromSpawnType(SpawnEvent type) {
		switch (type) {
		case SpawnEvent::kDraugr:
			return GetRandomNPCFromList(Forms::Loader::cont_spawn_vec_draugr, Forms::Loader::container_spawn_draugr);
		case SpawnEvent::kDwarven:
			return GetRandomNPCFromList(Forms::Loader::cont_spawn_vec_dwarven, Forms::Loader::container_spawn_dwarven);
		case SpawnEvent::kWarlock:
			return GetRandomNPCFromList(Forms::Loader::cont_spawn_vec_warlock, Forms::Loader::container_spawn_warlock);
		case SpawnEvent::kGeneric:
			return GetRandomNPCFromList(Forms::Loader::cont_spawn_vec_generic, Forms::Loader::container_spawn_mimic);
		case SpawnEvent::kNPCWerewolf:
			return GetRandomNPCFromList(Forms::Loader::npc_spawn_vec_werefolf, Forms::Loader::npc_spawn_werewolf);
		case SpawnEvent::kNPCGeneric:
			return GetRandomNPCFromList(Forms::Loader::npc_spawn_generic_vec);
		case SpawnEvent::kNPCVampire:
			return GetRandomNPCFromList(Forms::Loader::npc_spawn_vec_vampire);
		case SpawnEvent::kNPCUndead:
			return GetRandomNPCFromList(Forms::Loader::npc_spawn_vec_undead);
		case SpawnEvent::kNPCDwarven:
			return GetRandomNPCFromList(Forms::Loader::npc_spawn_vec_dwarven);
		case SpawnEvent::kNPCDragon:
			return GetRandomNPCFromList(Forms::Loader::npc_spawn_vec_dragon);
		default:
			return nullptr;
		}
	}

	inline static RE::TESObjectREFR* PapyrusPlaceAtMe(RE::TESObjectREFR* target, RE::TESForm* object, bool persistent, bool disabled) {
		if (target && object) {
			if (object->GetFormType() == RE::FormType::Explosion && disabled) {
				// disabled explosions not allowed
			}
			else {
				using func_t = RE::TESObjectREFR* (*)(std::int64_t, std::int32_t, RE::TESObjectREFR*, RE::TESForm*, std::int32_t, bool, bool);
				static REL::Relocation<func_t> func{ REL::ID(56203) };
				return func(NULL, NULL, target, object, 1, persistent, disabled);
			}
		}
		return nullptr;
	}
};
