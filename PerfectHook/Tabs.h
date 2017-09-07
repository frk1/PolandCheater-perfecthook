#pragma once
#include "IDirect3DDevice9.h"
typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;

void KnifeApplyCallbk()
{
	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(U::FindPattern("engine.dll", reinterpret_cast<PBYTE>("\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"), "x????x????xxxxxxx"));
	CL_FullUpdate();
}
void dankestSetClanTag(const char* tag, const char* name)
{
	static auto ClanTagOffset = U::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx");
	if (ClanTagOffset)
	{
		if (strlen(name) > 0) {
			auto name_ = std::string(name);
			auto newline = name_.find("\\n");
			auto tab = name_.find("\\t");
			if (newline != std::string::npos) {
				name_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				name_.replace(tab, tab + 2, "\t");
			}
		}
		auto tag_ = std::string(tag);
		if (strlen(tag) > 0) {
			auto newline = tag_.find("\\n");
			auto tab = tag_.find("\\t");
			if (newline != std::string::npos) {
				tag_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				tag_.replace(tab, tab + 2, "\t");
			}
		}
		static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
		dankesttSetClanTag(tag_.data(), tag_.data());
	}
}

void RenderRageBot()
{
	ImGui::Checkbox(("Active"), &menu.Ragebot.b1g);

	ImGui::Columns(3, NULL, true);
	{
		ImGui::Text("General"); ImGui::NextColumn(); ImGui::Text("Accuracy"); ImGui::NextColumn(); ImGui::Text("Anti-Aim"); ImGui::NextColumn(); ImGui::Separator();
		ImGui::Checkbox(("Enabled"), &menu.Ragebot.Enabled);
		ImGui::Checkbox(("Auto Fire"), &menu.Ragebot.AutoFire);
		ImGui::SliderFloat(("FOV"), &menu.Ragebot.FOV, 1.f, 180.f, "%.0f");
		ImGui::Checkbox(("Silent Aim"), &menu.Ragebot.Silent);
		ImGui::Checkbox(("AutoPistol"), &menu.Ragebot.AutoPistol);
		ImGui::Hotkey(("Key##999"), &menu.Ragebot.KeyPress);
		ImGui::Text(("Accuracy"));
		//ImGui::Separator();
		ImGui::Checkbox(("Friendly Fire"), &menu.Ragebot.FriendlyFire);
		ImGui::Combo(("Hitbox"), &menu.Ragebot.Hitbox, aimBones, ARRAYSIZE(aimBones));
		ImGui::Combo(("Hitscan"), &menu.Ragebot.Hitscan, hitscan, ARRAYSIZE(hitscan));
        
		//	ImGui::Separator();

	}
	ImGui::NextColumn();
	{

		//	ImGui::Text(("Accuracy"));
		//ImGui::Separator();

		ImGui::Checkbox(("NoRecoil"), &menu.Ragebot.AntiRecoil);
		ImGui::Checkbox(("Auto Stop"), &menu.Ragebot.AutoStop);
		ImGui::Checkbox(("Auto Crouch"), &menu.Ragebot.AutoCrouch);
		ImGui::Checkbox(("Auto Scope"), &menu.Ragebot.AutoScope);
		ImGui::SliderFloat(("Minimum Damage"), &menu.Ragebot.MinimumDamage, 1.f, 100.f, "%.2f");
		ImGui::Checkbox(("Hitchance"), &menu.Ragebot.Hitchance);
		ImGui::SliderFloat(("Hitchance Amount"), &menu.Ragebot.HitchanceAmount, 0.f, 100.f, "%.1f");
		ImGui::Checkbox(("Resolver"), &menu.Ragebot.Resolver);
        ImGui::Checkbox("LBY Backtracking", &menu.Ragebot.FakeLagFix);
        ImGui::Hotkey(("BAIM key"), &menu.Ragebot.BAIMkey);


	}
	ImGui::NextColumn();
	{

		//	ImGui::Text("Anti-Aim");
		//	ImGui::Separator();
		ImGui::Checkbox(("Anti-Aim Enabled"), &menu.Ragebot.EnabledAntiAim);
		ImGui::Combo(("Pitch"), &menu.Ragebot.Pitch, antiaimpitch, ARRAYSIZE(antiaimpitch));
		ImGui::Combo(("Yaw"), &menu.Ragebot.YawTrue, antiaimyawtrue, ARRAYSIZE(antiaimyawtrue));
		ImGui::Combo(("Yaw-Fake"), &menu.Ragebot.YawFake, antiaimyawfake, ARRAYSIZE(antiaimyawfake));
		ImGui::Checkbox(("At Players"), &menu.Ragebot.AtTarget);
		ImGui::Checkbox(("Edge"), &menu.Ragebot.Edge);
		ImGui::Checkbox(("AntiAim on knife"), &menu.Ragebot.KnifeAA);
	}
}

void RenderLegit()
{
	ImGui::Checkbox(("Active"), &menu.Legitbot.b1g);
	ImGui::Columns(5, nullptr, true);
	{
		ImGui::Text("General"); ImGui::NextColumn(); ImGui::Text("Rifles"); ImGui::NextColumn(); ImGui::Text("Pistol"); ImGui::NextColumn(); ImGui::Text("Sniper"); ImGui::NextColumn(); ImGui::Text("Triggerbot"); ImGui::NextColumn(); ImGui::Separator();

		ImGui::Checkbox(("Enable Aimbot"), &menu.Legitbot.Aimbot.Enabled);
		//ImGui::Checkbox(("Auto Pistol"), &menu.Legitbot.Aimbot.AutoPistol);
		ImGui::Checkbox(("Backtrack"), &menu.Legitbot.backtrack);
	}
	//ImGui::Separator();
	ImGui::NextColumn();

	{
        ImGui::Hotkey("Key##0", &menu.Legitbot.MainKey);
        //ImGui::Combo(("Key##0"), &menu.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
        ImGui::SliderFloat("Smooth##0", &menu.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
        ImGui::SliderFloat("FOV##0", &menu.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
        ImGui::SliderFloat("Min RCS##0", &menu.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
        ImGui::SliderFloat("Max RCS##0", &menu.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");
	}

	ImGui::NextColumn();

	{
        ImGui::Hotkey("Key##1", &menu.Legitbot.PistolKey);
        //ImGui::Combo(("Key##1"), &menu.Legitbot.PistolKey, keyNames, ARRAYSIZE(keyNames));
        ImGui::SliderFloat("Smooth##1", &menu.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
        ImGui::SliderFloat("FOV##1", &menu.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");
        ImGui::SliderFloat("Min RCS##1", &menu.Legitbot.pistol_recoil_min, 1.00f, 100.00f, "%.2f");
        ImGui::SliderFloat("Max RCS##1", &menu.Legitbot.pistol_recoil_max, 1.00f, 100.00f, "%.2f");
	}
	ImGui::NextColumn();

	{
        ImGui::Hotkey("Key##2", &menu.Legitbot.SniperKey);
        //ImGui::Combo(("Key##2"), &menu.Legitbot.SniperKey, keyNames, ARRAYSIZE(keyNames));
        ImGui::SliderFloat("Smooth##2", &menu.Legitbot.SniperSmooth, 1.00f, 100.00f, "%.2f");
        ImGui::SliderFloat("FOV##2", &menu.Legitbot.Sniperfov, 0.00f, 30.00f, "%.2f");
        ImGui::SliderFloat("Min RCS##2", &menu.Legitbot.sniper_recoil_min, 1.00f, 100.00f, "%.2f");
        ImGui::SliderFloat("Max RCS##2", &menu.Legitbot.sniper_recoil_max, 1.00f, 100.00f, "%.2f");
	}

	ImGui::NextColumn();

	{
		//ImGui::Separator();
		ImGui::Checkbox(("Enable Triggerbot"), &menu.Legitbot.Triggerbot.Enabled);
		ImGui::SliderFloat(("Delay"), &menu.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
        ImGui::Hotkey("Key##3", &menu.Legitbot.Triggerbot.Key);
        //ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
		ImGui::SliderFloat(("Hitchance"), &menu.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");

		ImGui::Text(("Filter"));

		ImGui::BeginChild(("Filter"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 19 * 5));
		{
			ImGui::Selectable((" Head"), &menu.Legitbot.Triggerbot.Filter.Head);
			ImGui::Selectable((" Arms"), &menu.Legitbot.Triggerbot.Filter.Arms);
			ImGui::Selectable((" Chest"), &menu.Legitbot.Triggerbot.Filter.Chest);
			ImGui::Selectable((" Stomach"), &menu.Legitbot.Triggerbot.Filter.Stomach);
			ImGui::Selectable((" Legs"), &menu.Legitbot.Triggerbot.Filter.Legs);
		} ImGui::EndChild();
	}
}

void RenderVisuals()
{
	ImGui::Checkbox(("Active"), &menu.Visuals.Enabled);

	ImGui::Columns(3, NULL, true);
	{
		ImGui::Text("Player"); ImGui::NextColumn(); ImGui::Text("Filter"); ImGui::NextColumn(); ImGui::Text("Misc"); ImGui::NextColumn(); ImGui::Separator();
		ImGui::Checkbox(("Box"), &menu.Visuals.Box);
		ImGui::Checkbox(("Name"), &menu.Visuals.Name);
		ImGui::Checkbox(("HP"), &menu.Visuals.HP);
		ImGui::Checkbox(("Weapon"), &menu.Visuals.Weapon);
		ImGui::Checkbox(("Chams"), &menu.Visuals.Chams);
		ImGui::Checkbox(("DLight"), &menu.Visuals.DLight);
		ImGui::Checkbox(("SpreadCrosshair"), &menu.Visuals.SpreadCrosshair);
		ImGui::Checkbox(("GrenadeESP"), &menu.Visuals.GrenadeESP);
		ImGui::Checkbox(("C4"), &menu.Visuals.C4);
        //ImGui::Checkbox("Glow", &menu.Visuals.Glow);
	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild(("Filter"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 19 * 2));
		{
			ImGui::Selectable((" Players"), &menu.Visuals.Filter.Players);
			ImGui::Selectable((" Enemy Only"), &menu.Visuals.Filter.EnemyOnly);
		} ImGui::EndChild();
		ImGui::Text("World");
		ImGui::Checkbox(("C4 - World"), &menu.Visuals.C4World);
		ImGui::Checkbox(("Weapons - World"), &menu.Visuals.WeaponsWorld);



	}
	ImGui::NextColumn();
	{


		ImGui::Checkbox(("No Visual Recoil"), &menu.Visuals.NoVisualRecoil);
		ImGui::Combo(("Hands"), &menu.Visuals.Hands, handd, ARRAYSIZE(handd));
		ImGui::SliderFloat(("FOV"), &menu.Visuals.FOVChanger, 0.f, 60.f, "%.0f");
		ImGui::SliderFloat(("viewmodel FOV"), &menu.Visuals.viewmodelChanger, 0.f, 120.f, "%.0f");
		ImGui::Checkbox(("NoFlash"), &menu.Visuals.NoFlash);
        ImGui::Checkbox(("NoSmoke"), &menu.Visuals.NoSmoke);
		ImGui::Checkbox(("Time"), &menu.Visuals.Time);
		ImGui::Checkbox(("noScope Border"), &menu.Visuals.noscopeborder);
		ImGui::Checkbox(("Grenade Prediction"), &menu.Visuals.GrenadePrediction);
        ImGui::Checkbox(("ThirdPerson"), &menu.Visuals.ThirdPerson);
	}
}
bool memesaredank;

void RenderMisc()
{


    ImGui::Checkbox("Name Stealer", &menu.Misc.silentstealer);
    if(ImGui::Button("Silent exploit"))
    {
        static ConVar* name = I::CVar->FindVar("name");
        if (name)
        {
            *(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
            name->SetValue("\nญญญ");
        }
    }
    ImGui::Checkbox(("Bunny Hop"), &menu.Misc.Bhop);
    ImGui::Checkbox(("AutoStrafe"), &menu.Misc.AutoStrafe);
    ImGui::Checkbox(("NightMode"), &menu.Misc.nightMode);
    ImGui::Checkbox(("SpecList"), &menu.Misc.SpecList);
    ImGui::Combo(("Spammer"), &menu.Misc.spammer, spammers, ARRAYSIZE(spammers));
    ImGui::Checkbox("phook clantag", &menu.Misc.syncclantag);
    ImGui::PushItemWidth(100);
    ImGui::Hotkey("ragequit key", &menu.Misc.ragequit);
    if (ImGui::Button("Spoof mat_picmip")) menu.Misc.SpoofConfirmation = true;
    if (menu.Misc.SpoofConfirmation)
    {
        ImGuiStyle& meme = ImGui::GetStyle();

        meme.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        meme.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        ImGui::Begin(("Confirmation Menu"), &menu.Misc.SpoofConfirmation, ImVec2(225, 120), 0.9f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        {
            if (!I::Engine->IsInGame())
            {
                ImGui::Text("Do you want to spoof mat_picmip?");
                ImGui::Columns(2, ("##memes"), false);
                if (ImGui::Button(("Yes"), ImVec2(93.f, 24.f)))
                {
                    auto drawmemes = I::CVar->FindVar("mat_picmip");
                    auto drawmemesenabled = new SpoofedConvar(drawmemes, "newdrawmemesaredank");
                    drawmemesenabled->SetInt(4);
                    menu.Misc.SpoofConfirmation = false;
                }
                ImGui::NextColumn();
                if (ImGui::Button(("No"), ImVec2(93.f, 24.f))) menu.Misc.SpoofConfirmation = false;
                ImGui::Columns(1);
            }
            else
            {
                ImGui::Text("You can't use it in game.");
                if (ImGui::Button(("Ok"), ImVec2(200.f, 24.f))) menu.Misc.SpoofConfirmation = false;
            }
            ImGui::End();
        }
    }
    ImGui::Text(("Custom Name"));
    ImGui::PushItemWidth(180);
    static char buf1[128] = ""; ImGui::InputText(("##Name"), buf1, 128, ImGuiInputTextFlags_AllowTabInput);
    ImGui::SameLine();
    ImGui::PushItemWidth(50);
    if (ImGui::Button(("Change Name")))
    {
        ConVar* name = I::CVar->FindVar("name");
        if (name)
        {
            *(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
            name->SetValue(buf1);
        }
    }
    ImGui::Text(("Custom ClanTag"));
    ImGui::PushItemWidth(180);
    static char buf2[128] = ""; ImGui::InputText(("##Clantag"), buf2, 16, ImGuiInputTextFlags_AllowTabInput);
    ImGui::SameLine();
    ImGui::PushItemWidth(50);
    if (ImGui::Button(("Change Clantag")))
    {
        dankestSetClanTag(buf2, buf2);
    }

    ImGui::Text(("Animated ClanTag"));
    ImGui::PushItemWidth(180);
    ImGui::InputText(("##AnimatedClantag"), G::AnimatedClantag, 16, ImGuiInputTextFlags_AllowTabInput);
    ImGui::SameLine();
    ImGui::PushItemWidth(50);
    if (ImGui::Button(("Start")))
    {
        menu.Misc.animatedclantag = true;
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(50);
    if (ImGui::Button(("Stop")))
    {
        menu.Misc.animatedclantag = false;
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(200);
    ImGui::SliderInt(("Speed"), &menu.Misc.ClanTagSpeed, 0, 1000, "%.0f");

    ImGui::Text("Config");
    ImGui::Combo(("File"), &menu.Menu.ConfigFile, configFiles, ARRAYSIZE(configFiles));
    if (ImGui::Button("Save Config")) Config->Save();
    ImGui::SameLine();
    if (ImGui::Button("Load Config")) Config->Load();
    ImGui::SameLine();
    if (ImGui::Button("ragequit"))
    {
        I::Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
        DoUnload = true;
    }


}

void RenderSkinChanger()
{
    {
        ImGui::Checkbox(("Enabled"), &menu.Skinchanger.Enabled);
        ImGui::SameLine();
        ImGui::PushItemWidth(150);
        if (ImGui::Button(("Force Update")))
        {
            KnifeApplyCallbk();
        }


        //ImGui::Separator();

        ImGui::Text("General");
        ImGui::Combo(("Knife Model"), &menu.Skinchanger.Knife, knives, ARRAYSIZE(knives));
        ImGui::Combo(("Knife Skin"), &menu.Skinchanger.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));
        ImGui::Combo(("Gloves"), &menu.Skinchanger.gloves, gloves, ARRAYSIZE(gloves));


        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::Text("Rifles");
        ImGui::Columns(2, NULL, false);
        ImGui::Combo(("AK-47"), &menu.Skinchanger.AK47Skin, ak47, ARRAYSIZE(ak47));
        ImGui::Combo(("M4A1-S"), &menu.Skinchanger.M4A1SSkin, m4a1s, ARRAYSIZE(m4a1s));
        ImGui::Combo(("M4A4"), &menu.Skinchanger.M4A4Skin, m4a4, ARRAYSIZE(m4a4));
        ImGui::NextColumn();
        ImGui::Combo(("AUG"), &menu.Skinchanger.AUGSkin, aug, ARRAYSIZE(aug));
        ImGui::Combo(("FAMAS"), &menu.Skinchanger.FAMASSkin, famas, ARRAYSIZE(famas));

        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::Text("Snipers");
        ImGui::Columns(2, NULL, false);
        ImGui::Combo(("AWP"), &menu.Skinchanger.AWPSkin, awp, ARRAYSIZE(awp));
        ImGui::Combo(("SSG08"), &menu.Skinchanger.SSG08Skin, ssg08, ARRAYSIZE(ssg08));
        ImGui::NextColumn();
        ImGui::Combo(("SCAR20"), &menu.Skinchanger.SCAR20Skin, scar20, ARRAYSIZE(scar20));

        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::Text("SMG's");
        ImGui::Columns(2, NULL, false);
        ImGui::Combo(("P90"), &menu.Skinchanger.P90Skin, p90, ARRAYSIZE(p90));
        ImGui::NextColumn();
        ImGui::Combo(("UMP45"), &menu.Skinchanger.UMP45Skin, ump45, ARRAYSIZE(ump45));

        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::Text("Pistols");
        ImGui::Columns(2, NULL, false);
        ImGui::Combo(("Glock-18"), &menu.Skinchanger.GlockSkin, glock, ARRAYSIZE(glock));
        ImGui::Combo(("USP-S"), &menu.Skinchanger.USPSkin, usp, ARRAYSIZE(usp));
        ImGui::Combo(("Deagle"), &menu.Skinchanger.DeagleSkin, deagle, ARRAYSIZE(deagle));

        ImGui::NextColumn();
        ImGui::Combo(("TEC-9"), &menu.Skinchanger.tec9Skin, tec9, ARRAYSIZE(tec9));
        ImGui::Combo(("P2000"), &menu.Skinchanger.P2000Skin, p2000, ARRAYSIZE(p2000));
        ImGui::Combo(("P250"), &menu.Skinchanger.P250Skin, p250, ARRAYSIZE(p250));

    }
}
