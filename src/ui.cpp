#include "ui.h"

namespace UI
{

bool Register()
{
    if (!SKSEMenuFramework::IsInstalled())
    {
        return false;
    }
    SKSEMenuFramework::SetSection(MenuTitles::ModName);
    SKSEMenuFramework::AddSectionItem(MenuTitles::Settings, Settings::RenderSettings);
    SKSEMenuFramework::AddSectionItem(MenuTitles::Toggles, Toggles::RenderToggles);
    RestoreSettingsFromConfig(true, true);

    return true;
}

void RestoreSettingsFromConfig(bool settings, bool toggles)
{
    using s = Config::Settings;
    if (settings)
    {
        Settings::spawn_chance_temp = s::mimic_chance.GetValue();
        Settings::use_delayed_explosion_temp = s::delay_explosion_active.GetValue();
        Settings::use_time_ranges_temp = s::delay_time_range_active.GetValue();
        Settings::delay_time_without_delay_temp = (float)s::delay_timer_seconds.GetValue();
        Settings::delay_time_range_minimum_temp = (float)s::delay_time_min.GetValue();
        Settings::delay_time_range_maximum_temp = (float)s::delay_time_max.GetValue();
        Settings::spawn_from_formlist = s::spawn_from_formlist.GetValue();
    }
    if (toggles)
    {
        Toggles::toggle_npc_spawns_temp = s::npc_spawn_generic.GetValue();
        Toggles::toggle_werewolf_spawns_temp = s::npc_spawn_werewolf.GetValue();
        Toggles::toggle_draugr_temp = s::container_spawn_draugr_active.GetValue();
        Toggles::toggle_dwarven_temp = s::container_spawn_dwarven_active.GetValue();
        Toggles::toggle_warlock_temp = s::container_spawn_warlock_active.GetValue();
        Toggles::toggle_mimic_temp = s::container_spawn_mimic_active.GetValue();
        Toggles::toggle_urn_temp = s::explosion_spawn_urn.GetValue();
        Toggles::toggle_meme_sound_temp = s::meme_sound_active.GetValue();
        Toggles::toggle_explosion_visuals_temp = s::visual_explosions_for_spawns_active.GetValue();
        Toggles::toggle_npc_vampire = s::npc_spawn_vampire.GetValue();
        Toggles::npc_toggle_undead = s::npc_spawn_undead.GetValue();
        Toggles::npc_toggle_dwarven = s::npc_spawn_dwarven.GetValue();
        Toggles::npc_toggle_dragon = s::npc_spawn_dragon.GetValue();
		Toggles::spawn_from_ore = s::spawn_from_ore_vein.GetValue();
    }
}

void RestoreDefaultSettings(bool settings, bool toggles)
{
    using s = Config::Settings;
    if (settings)
    {
        Settings::spawn_chance_temp = 10.0;
        Settings::use_delayed_explosion_temp = true;
        Settings::use_time_ranges_temp = true;
        Settings::delay_time_without_delay_temp = 2.5;
        Settings::delay_time_range_minimum_temp = 1.0;
        Settings::delay_time_range_maximum_temp = 10.0;
        Settings::spawn_from_formlist = true;

        s::mimic_chance.SetValue(Settings::spawn_chance_temp);
        s::delay_timer_seconds.SetValue(Settings::delay_time_without_delay_temp);
        s::delay_explosion_active.SetValue(Settings::use_delayed_explosion_temp);
        s::delay_time_range_active.SetValue(Settings::use_time_ranges_temp);
        s::delay_time_min.SetValue(Settings::delay_time_range_minimum_temp);
        s::delay_time_max.SetValue(Settings::delay_time_range_maximum_temp);
        s::spawn_from_formlist.SetValue(Settings::spawn_from_formlist);
    }
    if (toggles)
    {
        Toggles::toggle_npc_spawns_temp = true;
        Toggles::toggle_werewolf_spawns_temp = true;
        Toggles::toggle_draugr_temp = true;
        Toggles::toggle_dwarven_temp = true;
        Toggles::toggle_warlock_temp = true;
        Toggles::toggle_mimic_temp = true;
        Toggles::toggle_urn_temp = true;
        Toggles::toggle_meme_sound_temp = false;
        Toggles::toggle_explosion_visuals_temp = true;
        Toggles::toggle_npc_vampire = true;
        Toggles::npc_toggle_undead = true;
        Toggles::npc_toggle_dwarven = true;
        Toggles::npc_toggle_dragon = false;
		Toggles::spawn_from_ore = true;

        s::npc_spawn_generic.SetValue(Toggles::toggle_npc_spawns_temp);
        s::npc_spawn_generic.SetValue(Toggles::toggle_npc_spawns_temp);
        s::npc_spawn_werewolf.SetValue(Toggles::toggle_werewolf_spawns_temp);
        s::container_spawn_draugr_active.SetValue(Toggles::toggle_draugr_temp);
        s::container_spawn_dwarven_active.SetValue(Toggles::toggle_dwarven_temp);
        s::container_spawn_warlock_active.SetValue(Toggles::toggle_warlock_temp);
        s::container_spawn_mimic_active.SetValue(Toggles::toggle_mimic_temp);
        s::explosion_spawn_urn.SetValue(Toggles::toggle_urn_temp);
        s::meme_sound_active.SetValue(Toggles::toggle_meme_sound_temp);
        s::visual_explosions_for_spawns_active.SetValue(Toggles::toggle_explosion_visuals_temp);
        s::npc_spawn_vampire.SetValue(Toggles::toggle_npc_vampire);
        s::npc_spawn_undead.SetValue(Toggles::npc_toggle_undead);
        s::npc_spawn_dwarven.SetValue(Toggles::npc_toggle_dwarven);
        s::npc_spawn_dragon.SetValue(Toggles::npc_toggle_dragon);
		s::spawn_from_ore_vein.SetValue( Toggles::spawn_from_ore );
    }

    Config::Settings::GetSingleton()->UpdateSettings(true);
}

void HelpMarker(const char *desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

namespace Settings
{
void __stdcall RenderSettings()
{
    using s = Config::Settings;

    FontAwesome::PushSolid();
    ImGui::TextColored(ImVec4(0.2f, 0.85f, 0.4f, 1.0f), MenuTitles::Settings.c_str());

    ImGui::SetNextItemWidth(300.f);
	if ( ImGui::SliderFloat( Label::spawn_chance.c_str(), &spawn_chance_temp, 0.0, 100.0, "%.2f%%" ) )
    {
        s::mimic_chance.SetValue(spawn_chance_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::spawn_chance.c_str());

    if (ImGui::Checkbox(Label::use_delay_explosion_toggle.c_str(), &use_delayed_explosion_temp))
    {
        s::delay_explosion_active.SetValue(use_delayed_explosion_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::use_delay_explosion_toggle.c_str());
    ImGui::SameLine();
    if (ImGui::Checkbox(Label::use_delay_time_ranges.c_str(), &use_time_ranges_temp))
    {
        s::delay_time_range_active.SetValue(use_time_ranges_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::use_delay_time_ranges.c_str());

    if (ImGui::Checkbox(Label::spawn_from_formlist.c_str(), &spawn_from_formlist))
    {
        s::spawn_from_formlist.SetValue(spawn_from_formlist);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::spawn_from_formlist.c_str());

    ImGui::NewLine();

    ImGui::TextColored(ImVec4(0.2f, 0.85f, 0.4f, 1.0f), MenuTitles::Timers.c_str());

    ImGui::SetNextItemWidth(300.f);
	if ( ImGui::SliderFloat( Label::delay_time_without_range.c_str(), &delay_time_without_delay_temp, 0.0f, 20.0f, "%.2fsec" ) )
    {
        s::delay_timer_seconds.SetValue(delay_time_without_delay_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::delay_time_without_range.c_str());

    ImGui::SetNextItemWidth(300.f);
    if (ImGui::SliderFloat(Label::delay_time_min.c_str(), &delay_time_range_minimum_temp, 0.0f, delay_time_range_maximum_temp, "%.2fsec" ) )
    {
        s::delay_timer_seconds.SetValue(delay_time_range_minimum_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::delay_time_min.c_str());

    ImGui::SetNextItemWidth(300.f);
    if (ImGui::SliderFloat(Label::delay_time_max.c_str(), &delay_time_range_maximum_temp, delay_time_range_minimum_temp, 25.0f, "%.2fsec" ) )
    {
        s::delay_timer_seconds.SetValue(delay_time_range_maximum_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::delay_time_max.c_str());

    // Save Config and Default Settings
    ImGui::NewLine();
    ImGui::TextColored(ImVec4(0.2f, 0.85f, 0.4f, 1.0f), MenuTitles::System.c_str());
    if (ImGui::Button(MenuTitles::Save.c_str()))
    {
        Config::Settings::GetSingleton()->UpdateSettings(true);
    }
    ImGui::SameLine();
    if (ImGui::Button(MenuTitles::Restore.c_str()))
    {
        RestoreDefaultSettings(true, false);
    }
    FontAwesome::Pop();
}
} // namespace Settings
namespace Toggles
{
void __stdcall RenderToggles()
{
    using s = Config::Settings;
    using namespace Toggles;

    FontAwesome::PushSolid();
    ImGui::TextColored(ImVec4(0.2f, 0.85f, 0.4f, 1.0f), "Spawn Toggles");

    ImGui::NewLine();
    ImGui::TextColored(ImVec4(0.2f, 0.85f, 0.4f, 1.0f), MenuTitles::Misc.c_str());
    // line 1
    if (ImGui::Checkbox(Label::toggle_meme_sound.c_str(), &toggle_meme_sound_temp))
    {
        s::meme_sound_active.SetValue(toggle_meme_sound_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_meme_sound.c_str());
    ImGui::SameLine();
    if (ImGui::Checkbox(Label::toggle_explosion_visuals.c_str(), &toggle_explosion_visuals_temp))
    {
        s::visual_explosions_for_spawns_active.SetValue(toggle_explosion_visuals_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_explosion_visuals.c_str());

    //line 2 misc
	if ( ImGui::Checkbox( Label::spawn_from_ore.c_str(), &spawn_from_ore ) )
	{
		s::spawn_from_ore_vein.SetValue( spawn_from_ore );
    }
	ImGui::SameLine();
	HelpMarker( Tooltip::spawn_from_ore.c_str() );

    ImGui::NewLine();
    ImGui::TextColored(ImVec4(0.2f, 0.85f, 0.4f, 1.0f), MenuTitles::Spawns.c_str());

    ImGui::Text("NPC Spawns");

    // line 1 spawns npcs
    if (ImGui::Checkbox(Label::toggle_npc_spawns.c_str(), &toggle_npc_spawns_temp))
    {
        s::npc_spawn_generic.SetValue(toggle_npc_spawns_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_npc_spawns.c_str());
    ImGui::SameLine();
    if (ImGui::Checkbox(Label::toggle_werewolf_spawns.c_str(), &toggle_werewolf_spawns_temp))
    {
        s::npc_spawn_werewolf.SetValue(toggle_werewolf_spawns_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_werewolf_spawns.c_str());

    // line 2 spawns npcs
    if (ImGui::Checkbox(Label::toggle_npc_vampires.c_str(), &toggle_npc_vampire))
    {
        s::npc_spawn_vampire.SetValue(toggle_npc_vampire);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_npc_vampires.c_str());
    ImGui::SameLine();
    if (ImGui::Checkbox(Label::npc_toggle_undead.c_str(), &npc_toggle_undead))
    {
        s::npc_spawn_undead.SetValue(npc_toggle_undead);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::npc_toggle_undead.c_str());
    // line 3 npcs

    if (ImGui::Checkbox(Label::npc_toggle_dwarven.c_str(), &npc_toggle_dwarven))
    {
        s::npc_spawn_dwarven.SetValue(npc_toggle_dwarven);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::npc_toggle_dwarven.c_str());
    ImGui::SameLine();
    if (ImGui::Checkbox(Label::npc_toggle_dragon.c_str(), &npc_toggle_dragon))
    {
        s::npc_spawn_dragon.SetValue(npc_toggle_dragon);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::npc_toggle_dragon.c_str());

    ImGui::Text("Containers");

    // line 1 cont
    if (ImGui::Checkbox(Label::toggle_draugr_spawns.c_str(), &toggle_draugr_temp))
    {
        s::container_spawn_draugr_active.SetValue(toggle_draugr_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_draugr_spawns.c_str());
    ImGui::SameLine();
    if (ImGui::Checkbox(Label::toggle_dwarven_spawns.c_str(), &toggle_dwarven_temp))
    {
        s::container_spawn_dwarven_active.SetValue(toggle_dwarven_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_dwarven_spawns.c_str());

    // line 2 cont
    if (ImGui::Checkbox(Label::toggle_warlock_spawns.c_str(), &toggle_warlock_temp))
    {
        s::container_spawn_warlock_active.SetValue(toggle_warlock_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_warlock_spawns.c_str());
    ImGui::SameLine();
    if (ImGui::Checkbox(Label::toggle_mimic_spawns.c_str(), &toggle_mimic_temp))
    {
        s::container_spawn_mimic_active.SetValue(toggle_mimic_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_mimic_spawns.c_str());

    // line 3 cont
    if (ImGui::Checkbox(Label::toggle_urn_spawns.c_str(), &toggle_urn_temp))
    {
        s::explosion_spawn_urn.SetValue(toggle_urn_temp);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_urn_spawns.c_str());

    // Save Config and Default Settings
    ImGui::NewLine();
    ImGui::TextColored(ImVec4(0.2f, 0.85f, 0.4f, 1.0f), MenuTitles::System.c_str());
    if (ImGui::Button(MenuTitles::Save.c_str()))
    {
        Config::Settings::GetSingleton()->UpdateSettings(true);
    }
    ImGui::SameLine();
    if (ImGui::Button(MenuTitles::Restore.c_str()))
    {
        RestoreDefaultSettings(false, true);
    }
    FontAwesome::Pop();
}

} // namespace Toggles

} // namespace UI
