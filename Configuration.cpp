#include "Configuration.hpp"
#include "Variables.h"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>
void CConfig::Setup()
{


    SetupValue(menu.Ragebot.b1g, false, ("Ragebot"), ("RageToggle"));
    SetupValue(menu.Ragebot.Enabled, false, ("Ragebot"), ("Enabled"));
    SetupValue(menu.Ragebot.AutoFire, false, ("Ragebot"), ("AutoFire"));
    SetupValue(menu.Ragebot.FOV, 0.f, ("Ragebot"), ("FOV"));
    SetupValue(menu.Ragebot.Silent, false, ("Ragebot"), ("Silent"));
    SetupValue(menu.Ragebot.AutoPistol, false, ("Ragebot"), ("AutoPistol"));
    SetupValue(menu.Ragebot.KeyPress, 0, ("Ragebot"), ("Key"));


    SetupValue(menu.Ragebot.EnabledAntiAim, false, ("Ragebot"), ("AntiaimEnabled"));
    SetupValue(menu.Ragebot.Pitch, 0, ("Ragebot"), ("AntiaimPitch"));
    SetupValue(menu.Ragebot.YawTrue, 0, ("Ragebot"), ("AntiaimYaw-true"));
    SetupValue(menu.Ragebot.YawFake, 0, ("Ragebot"), ("AntiaimYaw-fake"));
    SetupValue(menu.Ragebot.AtTarget, false, ("Ragebot"), ("attargets"));
    SetupValue(menu.Ragebot.Edge, false, ("Ragebot"), ("edge"));
    SetupValue(menu.Ragebot.KnifeAA, false, ("Ragebot"), ("KnifeAA"));

    SetupValue(menu.Ragebot.FriendlyFire, false, ("Ragebot"), ("FriendlyFire"));
    SetupValue(menu.Ragebot.Hitbox, 0, ("Ragebot"), ("Hitbox"));
    SetupValue(menu.Ragebot.Hitscan, 0, ("Ragebot"), ("Hitscan"));

    SetupValue(menu.Ragebot.AntiRecoil, false, ("Ragebot"), ("AntiRecoil"));
    SetupValue(menu.Ragebot.AutoStop, false, ("Ragebot"), ("AutoStop"));
    SetupValue(menu.Ragebot.AutoCrouch, false, ("Ragebot"), ("AutoCrouch"));
    SetupValue(menu.Ragebot.AutoScope, false, ("Ragebot"), ("AutoScope"));
    SetupValue(menu.Ragebot.MinimumDamage, 0.f, ("Ragebot"), ("AutoWallDamage"));
    SetupValue(menu.Ragebot.Hitchance, false, ("Ragebot"), ("HitChance"));
    SetupValue(menu.Ragebot.HitchanceAmount, 0.f, ("Ragebot"), ("HitChanceAmount"));
    SetupValue(menu.Ragebot.Resolver, false, ("Ragebot"), ("Resolver"));
    SetupValue(menu.Ragebot.FakeLagFix, false, ("Ragebot"), ("Backtrack-lby"));
    SetupValue(menu.Ragebot.BAIMkey, 0, ("Ragebot"), ("BAIMKey"));




	SetupValue(menu.Legitbot.b1g, false, ("Legitbot"), ("LegitToggle"));
	SetupValue(menu.Legitbot.Aimbot.Enabled, false, ("Legitbot"), ("Enabled"));
    SetupValue(menu.Legitbot.backtrack, false, ("Legitbot"), ("Backtrack"));


	SetupValue(menu.Legitbot.MainKey, 1, ("Legitbot"), ("Key"));
	SetupValue(menu.Legitbot.Mainfov, 0.f, ("Legitbot"), ("FOV"));
	SetupValue(menu.Legitbot.MainSmooth, 1.f, ("Legitbot"), ("Speed"));
	SetupValue(menu.Legitbot.main_recoil_min, 0, ("Legitbot"), ("RCS-min"));
    SetupValue(menu.Legitbot.main_recoil_max, 0, ("Legitbot"), ("RCS-max"));

	SetupValue(menu.Legitbot.PistolKey, 6, ("Legitbot"), ("Key-Pistol"));
	SetupValue(menu.Legitbot.Pistolfov, 0.f, ("Legitbot"), ("FOV-Pistol"));
	SetupValue(menu.Legitbot.PistolSmooth, 1.f, ("Legitbot"), ("Speed-Pistol"));
    SetupValue(menu.Legitbot.pistol_recoil_min, 0, ("Legitbot"), ("RCS-min-pistol"));
    SetupValue(menu.Legitbot.pistol_recoil_max, 0, ("Legitbot"), ("RCS-max-pistol"));

	SetupValue(menu.Legitbot.SniperKey, 6, ("Legitbot"), ("Key-Sniper"));
	SetupValue(menu.Legitbot.Sniperfov, 0.f, ("Legitbot"), ("FOV-Sniper"));
	SetupValue(menu.Legitbot.SniperSmooth, 0.f, ("Legitbot"), ("Speed-Sniper"));
    SetupValue(menu.Legitbot.sniper_recoil_min, 0, ("Legitbot"), ("RCS-min-sniper"));
    SetupValue(menu.Legitbot.sniper_recoil_max, 0, ("Legitbot"), ("RCS-max-sniper"));

	SetupValue(menu.Legitbot.Triggerbot.Enabled, false, ("Triggerbot"), ("Enabled"));
	SetupValue(menu.Legitbot.Triggerbot.Key, 6, ("Triggerbot"), ("Key"));
	SetupValue(menu.Legitbot.Triggerbot.Delay, 0.f, ("Triggerbot"), ("Delay"));
	SetupValue(menu.Legitbot.Triggerbot.hitchance, 0.f, ("Triggerbot"), ("Hitchance"));

	SetupValue(menu.Legitbot.Triggerbot.Filter.Head, false, ("Triggerbot"), ("FilterHead"));
	SetupValue(menu.Legitbot.Triggerbot.Filter.Chest, false, ("Triggerbot"), ("FilterChest"));
	SetupValue(menu.Legitbot.Triggerbot.Filter.Stomach, false, ("Triggerbot"), ("FilterStomach"));
	SetupValue(menu.Legitbot.Triggerbot.Filter.Arms, false, ("Triggerbot"), ("FilterArms"));
	SetupValue(menu.Legitbot.Triggerbot.Filter.Legs, false, ("Triggerbot"), ("FilterLegs"));





	SetupValue(menu.Visuals.Enabled, false, ("Visuals"), ("VisualsEnabled"));
	SetupValue(menu.Visuals.Box, false, ("Visuals"), ("Box"));
	SetupValue(menu.Visuals.Name, false, ("Visuals"), ("Name"));
	SetupValue(menu.Visuals.HP, false, ("Visuals"), ("HP"));
	SetupValue(menu.Visuals.Weapon, false, ("Visuals"), ("Weapon"));
	SetupValue(menu.Visuals.Chams, false, ("Visuals"), ("Chams"));
	SetupValue(menu.Visuals.DLight, false, ("Visuals"), ("DLight"));
	SetupValue(menu.Visuals.SpreadCrosshair, false, ("Visuals"), ("SpreadCrosshair"));
	SetupValue(menu.Visuals.GrenadeESP, false, ("Visuals"), ("GrenadeESP"));
    SetupValue(menu.Visuals.Glow, false, ("Visuals"), ("Glow"));

	SetupValue(menu.Visuals.NoVisualRecoil, false, ("Visuals"), ("NoVisualRecoil"));
	SetupValue(menu.Visuals.Hands, 0, ("Visuals"), ("Hands"));
    SetupValue(menu.Visuals.FOVChanger, 0.f, ("Visuals"), ("fovchanger"));
	SetupValue(menu.Visuals.viewmodelChanger, 68.f, ("Visuals"), ("viewmodel_fov"));
	SetupValue(menu.Visuals.NoFlash, false, ("Visuals"), ("NoFlash"));
	SetupValue(menu.Visuals.NoSmoke, false, ("Visuals"), ("NoSmoke"));
	SetupValue(menu.Visuals.Time, false, ("Visuals"), ("Time"));
	SetupValue(menu.Visuals.noscopeborder, false, ("Visuals"), ("noscopeborder"));
	SetupValue(menu.Visuals.C4, false, ("Visuals"), ("C4"));
    SetupValue(menu.Visuals.GrenadePrediction, false, ("Visuals"), ("GrenadePrediction"));
    SetupValue(menu.Visuals.ThirdPerson, false, ("Visuals"), ("thirdperson"));

	SetupValue(menu.Visuals.Filter.Players, false, ("Visuals"), ("Players"));
	SetupValue(menu.Visuals.Filter.EnemyOnly, false, ("Visuals"), ("EnemyOnly"));
	SetupValue(menu.Visuals.WeaponsWorld, false, ("Visuals"), ("WeaponsWorld"));
	SetupValue(menu.Visuals.C4World, false, ("Visuals"), ("C4World"));





	SetupValue(menu.Misc.Bhop, false, ("Misc"), ("Bhop"));
    SetupValue(menu.Misc.AutoStrafe, false, ("Misc"), ("Autostrafe"));
    SetupValue(menu.Misc.SpecList, false, ("Misc"), ("speclist"));
    SetupValue(menu.Misc.ragequit, 0, ("Misc"), ("ragequit-key"));


    SetupValue(menu.Skinchanger.Enabled, false, ("Skinchanger"), ("Enabled"));
    SetupValue(menu.Skinchanger.Knife, 0, ("SkinChanger"), ("Knife"));
    SetupValue(menu.Skinchanger.KnifeSkin, 0, ("SkinChanger"), ("KnifeSkin"));
    SetupValue(menu.Skinchanger.gloves, 0, ("SkinChanger"), ("gloves"));

    SetupValue(menu.Skinchanger.AK47Skin, 0, ("SkinChanger"), ("AK47Skin"));
    SetupValue(menu.Skinchanger.M4A1SSkin, 0, ("SkinChanger"), ("M4A1SSkin"));
    SetupValue(menu.Skinchanger.M4A4Skin, 0, ("SkinChanger"), ("M4A4Skin"));
    SetupValue(menu.Skinchanger.AUGSkin, 0, ("SkinChanger"), ("AUGSkin"));
    SetupValue(menu.Skinchanger.FAMASSkin, 0, ("SkinChanger"), ("FAMASSkin"));

    SetupValue(menu.Skinchanger.AWPSkin, 0, ("SkinChanger"), ("AWPSkin"));
    SetupValue(menu.Skinchanger.SSG08Skin, 0, ("SkinChanger"), ("SSG08Skin"));
    SetupValue(menu.Skinchanger.SCAR20Skin, 0, ("SkinChanger"), ("SCAR20Skin"));

    SetupValue(menu.Skinchanger.P90Skin, 0, ("SkinChanger"), ("P90Skin"));
    SetupValue(menu.Skinchanger.UMP45Skin, 0, ("SkinChanger"), ("UMP45Skin"));

    SetupValue(menu.Skinchanger.GlockSkin, 0, ("SkinChanger"), ("GlockSkin"));
    SetupValue(menu.Skinchanger.USPSkin, 0, ("SkinChanger"), ("USPSkin"));
    SetupValue(menu.Skinchanger.DeagleSkin, 0, ("SkinChanger"), ("DeagleSkin"));

    SetupValue(menu.Skinchanger.tec9Skin, 0, ("SkinChanger"), ("tec9Skin"));
    SetupValue(menu.Skinchanger.P2000Skin, 0, ("SkinChanger"), ("P2000Skin"));
    SetupValue(menu.Skinchanger.P250Skin, 0, ("SkinChanger"), ("P250Skin"));
}

void CConfig::SetupValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void CConfig::SetupValue(float &value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void CConfig::SetupValue(bool &value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{


		folder = std::string(path) + ("\\autismhack\\");
		switch (menu.Menu.ConfigFile)
		{
		case 0:
			file = std::string(path) + ("\\autismhack\\legit.ini");
			break;
		case 1:
			file = std::string(path) + ("\\autismhack\\rage.ini");
			break;
		}

	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\autismhack\\");
		switch (menu.Menu.ConfigFile)
		{
		case 0:
			file = std::string(path) + ("\\autismhack\\legit.ini");
			break;
		case 1:
			file = std::string(path) + ("\\autismhack\\rage.ini");
			break;
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = (float)atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config = new CConfig();
Variables menu;