
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "LagComp.h"
backtrackData headPositions[64][12];
const char* itemNames[] =
{
    "knife", //0 - default
    "deagle",
    "elite",
    "fiveseven",
    "glock",
    "none",
    "none",
    "ak47",
    "aug",
    "awp",

    "famas", //10
    "g3sg1",
    "none",
    "galil",
    "m249",
    "none",
    "m4a4",
    "mac-10",
    "none",
    "p90",

    "none", //20
    "none",
    "none",
    "none",
    "ump45",
    "xm1014",
    "bizon",
    "mag7",
    "negev",
    "sawed-off",

    "tec9", //30
    "taser",
    "p2000",
    "mp7",
    "mp9",
    "nova",
    "p250",
    "none",
    "scar20",
    "sg556",

    "ssg08", //40
    "knife",
    "knife",
    "flash",
    "nade",
    "smoke",
    "molotov",
    "decoy",
    "incendiary",
    "c4",

    "none", //50
    "none",
    "none",
    "none",
    "none",
    "none",
    "none",
    "none",
    "none",
    "knife",

    "m4a1", //60
    "usp",
    "none",
    "cz75",
    "revolver"
};

inline const char* ItemDefinitionIndexToString(int index)
{
    if (index < 0 || index > 64)
        index = 0;

    return itemNames[index];
}
float flGetDistance(Vector from, Vector to)
{
	Vector angle;
	angle.x = to.x - from.x;	angle.y = to.y - from.y;	angle.z = to.z - from.z;

	return sqrt(angle.x*angle.x + angle.y*angle.y + angle.z*angle.z);
}


void IESP::Init()
{
	BombCarrier = nullptr;
}
void IESP::CreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket)
{

}
int width = 0;
int height = 0;
float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

template <typename T, std::size_t N> T* end_(T(&arr)[N]) { return arr + N; }

template <typename T, std::size_t N> T* begin_(T(&arr)[N]) { return arr; }


bool done = false;
void IESP::PaintTraverse()
{


    {


        for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
        {

            IClientEntity *pEntity = g_EntityList->GetClientEntity(i);
            player_info_t pinfo;
            IClientEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
            if (pEntity == pLocal && pLocal->IsAlive() && g_Engine->GetPlayerInfo(g_Engine->GetLocalPlayer(), &pinfo))
            {
                if (g_Input->m_fCameraInThirdPerson && menu.Visuals.Enabled)
                {
                    Vector max = pEntity->GetCollideable()->OBBMaxs();
                    Vector pos, pos3D;
                    Vector top, top3D;
                    pos3D = pEntity->GetOrigin();
                    top3D = pos3D + Vector(0, 0, max.z);

                    if (!Render::WorldToScreen(pos3D, pos) || !Render::WorldToScreen(top3D, top))
                        return;

                    float height = (pos.y - top.y);
                    float width = height / 4.f;
                    if (menu.Visuals.Box)
                    {
                        Color color;
                        color = GetPlayerColor(pEntity);
                        PlayerBox(top.x, top.y, width, height, color);
                    }
                    if (menu.Visuals.HP)
                        DrawHealth(pos, top, (float)pLocal->GetHealth());

                    if (menu.Visuals.Name)
                        Render::DrawString2(Render::Fonts::ESP, (int)top.x, (int)top.y - 6, Color::White(), FONT_CENTER, pinfo.name);

                }
            }
            if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
            {
                if (g_Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
                {
                    if (menu.Legitbot.backtrack)
                    {
                        if (pLocal->IsAlive()) 
                        {
                            for (int t = 0; t < 12; ++t)
                            {
                                Vector screenbacktrack[64][12];

                                if (headPositions[i][t].simtime && headPositions[i][t].simtime > pLocal->GetSimulationTime() - 1)
                                {
                                    if (Render::WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
                                    {

                                        g_Surface->DrawSetColor(Color::Red());
                                        g_Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

                                    }
                                }
                            }
                        }
                        else
                        {
                            memset(&headPositions[0][0], 0, sizeof(headPositions));
                        }
                    }
                    if (menu.Visuals.Enabled && menu.Visuals.Filter.Players)
                        DrawPlayer(pEntity, pinfo, pLocal);
                }
                if (menu.Visuals.Enabled)
                {
                    ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();
                    if (menu.Visuals.WeaponsWorld && cClass->m_ClassID != (int)ClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47)))
                    {
                        DrawDrop(pEntity);
                    }
                    if (menu.Visuals.C4World)
                    {
                        if (cClass->m_ClassID == (int)ClassID::CPlantedC4)
                            DrawBombPlanted(pEntity, cClass);
                    }

                    if (cClass->m_ClassID == (int)ClassID::CC4)
                        DrawBomb(pEntity, cClass);
                    if (menu.Visuals.GrenadeESP && strstr(cClass->m_pNetworkName, "Projectile"))
                    {
                        DrawThrowable(pEntity);
                    }
                }
            }
        }


        grenade_prediction::instance().Paint();

        if (menu.Visuals.SpreadCrosshair)
        {
            IClientEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
            g_Engine->GetScreenSize(width, height);
            if (pLocal  && pLocal->IsAlive())
            {
                static Vector ViewAngles;
                g_Engine->GetViewAngles(ViewAngles);
                ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

                static Vector fowardVec;
                AngleVectors(ViewAngles, &fowardVec);
                fowardVec *= 10000;

                // Get ray start / end
                Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
                Vector end = start + fowardVec, endScreen;

                CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
                float cone = pWeapon->GetSpread() + pWeapon->GetInaccuracy();
                if (cone > 0.0f)
                {
                    if (cone < 0.01f) cone = 0.01f;
                    float size = (cone * height) * 0.7f;
                    Color color(255, 0, 0, 100);

                    if (Render::WorldToScreen(end, endScreen))
                    {
                        Render::OutlineCircle(endScreen.x, endScreen.y, (int)size, 48, color);
                    }
                }
            }
        }
    }


    if (menu.Misc.nightMode)
    {
        if (!done)
        {


            static auto sv_skyname = g_CVar->FindVar("sv_skyname");
            static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
            r_DrawSpecificStaticProp->SetValue(1);
            sv_skyname->SetValue("sky_csgo_night02");

            for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

                if (!pMaterial)
                    continue;

                const char* group = pMaterial->GetTextureGroupName();
                const char* name = pMaterial->GetName();

                if (strstr(group, "World textures"))
                {
                    pMaterial->ColorModulate(0.10, 0.10, 0.10);
                }
                if (strstr(group, "StaticProp"))
                {
                    pMaterial->ColorModulate(0.30, 0.30, 0.30);
                }
                if (strstr(name, "models/props/de_dust/palace_bigdome"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                }
                if (strstr(name, "models/props/de_dust/palace_pillars"))
                {
                    pMaterial->ColorModulate(0.30, 0.30, 0.30);
                }

                if (strstr(group, "Particle textures"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                }
                done = true;
            }

        }
    }
    else
    {
        if (done)
        {
            for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

                if (!pMaterial)
                    continue;

                const char* group = pMaterial->GetTextureGroupName();
                const char* name = pMaterial->GetName();

                if (strstr(group, "World textures"))
                {

                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(group, "StaticProp"))
                {

                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(name, "models/props/de_dust/palace_bigdome"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
                }
                if (strstr(name, "models/props/de_dust/palace_pillars"))
                {

                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(group, "Particle textures"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
                }
            }
            done = false;
        }
    }
}

std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size / 4, text, size);
	return wa;
}


C_CSPlayerResource* playerresources;
void IESP::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo, IClientEntity* pLocal)
{

	Vector max = pEntity->GetCollideable()->OBBMaxs();
	Vector pos, pos3D;
	Vector top, top3D;
	pos3D = pEntity->GetOrigin();
	top3D = pos3D + Vector(0, 0, max.z);

	if (!Render::WorldToScreen(pos3D, pos) || !Render::WorldToScreen(top3D, top))
		return;

	float height = (pos.y - top.y);
	float width = height / 4.f;

	Color color;

	if (menu.Visuals.Filter.EnemyOnly && (pEntity->GetTeamNum() == hack.pLocal()->GetTeamNum()))
		return;
	color = GetPlayerColor(pEntity);

	if (menu.Visuals.Box)
		PlayerBox(top.x, top.y, width, height, color);

	if (menu.Visuals.HP)
		DrawHealth(pos, top, (float)pEntity->GetHealth());

	if (menu.Visuals.Name)
		Render::DrawString2(Render::Fonts::ESP, (int)top.x, (int)top.y - 6, Color::White(), FONT_CENTER, pinfo.name);

	int bottom = 0;

	std::vector<std::string> weapon;
	std::vector<std::string> bomb;
	std::vector<std::string> rank;
	std::vector<std::string> wins;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
    if (menu.Visuals.Weapon && pWeapon)
    {
        int weapon_id = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();

        auto weapon_name = ItemDefinitionIndexToString(weapon_id);
        weapon.push_back(weapon_name);
    }



	if (menu.Visuals.C4 && pEntity == BombCarrier)
	{
		bomb.push_back("Bomb");
	}

	int i = 0;
	if (menu.Visuals.Weapon)
	{

		for (auto Text : weapon)
		{
			Render::DrawString2(Render::Fonts::Guns, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::White(), FONT_CENTER, "%s", Text.c_str());
			i++;
		}
	}
	if (menu.Visuals.C4)
	{
		for (auto Text : bomb)
		{
			Render::DrawString2(Render::Fonts::Guns, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::Red(), FONT_CENTER, Text.c_str());
			i++;
		}
	}

	/*if(menu.Visuals.money)
	{
		Render::Textf(int(top.x + width + 3), int(top.y + 12), Color(255, 255, 255, 255), Render::Fonts::ESP, "%i", pEntity->GetMoney());
	}*/



}


void IESP::PlayerBox(float x, float y, float w, float h, Color clr)
{
	g_Surface->DrawSetColor(clr);
	g_Surface->DrawOutlinedRect(int(x - w), int(y), int(x + w), int(y + h));
	g_Surface->DrawSetColor(Color::Black());
	g_Surface->DrawOutlinedRect(int(x - w - 1), int(y - 1), int(x + w + 1), int(y + h + 1));
	g_Surface->DrawOutlinedRect(int(x - w + 1), int(y + 1), int(x + w - 1), int(y + h - 1));
}

Color IESP::GetPlayerColor(IClientEntity* pEntity)
{
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis = MiscFunctions::IsVisible(hack.pLocal(), pEntity, (int)CSGOHitboxID::Head);

	Color color;

	if (TeamNum == TEAM_CS_T)
	{
		if (IsVis)
			color = Color(235, 200, 0, 255);
		else
			color = Color(235, 50, 0, 255);
	}
	else
	{
		if (IsVis)
			color = Color(235, 200, 0, 255);
		else
			color = Color(235, 50, 0, 255);
	}


	return color;
}

void IESP::DrawHealth(IClientEntity* pEntity, IESP::ESPBox size)
{
	int health = pEntity->GetHealth();
	int HP = health;
	if (HP > 100)
		HP = 100;
	int hp = health;
	float r = float(255 - health * 2.55);
	float g = float(health * 2.55);
	hp = (size.h - ((size.h * hp) / 100));

	Render::Outline(int(size.x - 4), int(size.y + hp), (int)2, int(size.h - hp + 1), Color((int)r, (int)g, 0));
	Render::Outline(int(size.x - 5), int(size.y - 1), (int)3, int(size.h + 2), Color(0, 0, 0, 150));
}

void IESP::DrawHealth(Vector bot, Vector top, float health)
{
	float h = (bot.y - top.y);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;

	UINT hp = UINT(h - (UINT)((h * health) / 100)); // Percentage

	int Red = int(255 - (health*2.55));
	int Green = int(health*2.55);

	Render::DrawOutlinedRect(int((top.x - offset) - 1), int(top.y - 1), 3, int(h + 2), Color::Black());

	Render::Line(int((top.x - offset)), int(top.y + hp), int((top.x - offset)), int(top.y + h), Color(Red, Green, 0, 255));
}

void IESP::DrawDrop(IClientEntity* pEntity)
{
    if (pEntity)
    {
        CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;

        auto owner = pWeapon->GetOwnerHandle();

        if (owner > -1)
            return;

        Vector pos3D = pEntity->GetAbsOrigin2();

        if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
            return;

        Vector pos;

        if (!Render::WorldToScreen(pos3D, pos))
            return;

        int weaponID = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
        auto weaponName = ItemDefinitionIndexToString(weaponID);


        Render::Text(pos.x, pos.y, Color(255, 255, 255, 255), Render::Fonts::DroppedGuns, weaponName);
    }
}
float damage;
int xd;
int meme;
char bombdamagestringdead[24];
char bombdamagestringalive[24];
void IESP::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;
	IClientEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	float flBlow = Bomb->GetC4BlowTime();
	float lifetime = flBlow - (g_Globals->interval_per_tick * pLocal->GetTickBase());
	if (Render::WorldToScreen(vOrig, vScreen))
	{
		if (pLocal->IsAlive())
		{
			float flDistance = pLocal->GetEyePosition().DistTo(pEntity->GetEyePosition());
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a*exp(-d * d);

			damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, pLocal->ArmorValue())), 0));

			sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "ur dead nigga");
			sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Health left: %.0f", pLocal->GetHealth() - damage);
			if (damage >= pLocal->GetHealth())
			{
				Render::Text((int)vScreen.x, int(vScreen.y + 10), Color(250, 42, 42, 255), Render::Fonts::Defuse, bombdamagestringdead);
			}
			else if (pLocal->GetHealth() > damage)
			{
				Render::Text((int)vScreen.x, int(vScreen.y + 10), Color(0, 255, 0, 255), Render::Fonts::Defuse, bombdamagestringalive);
			}
		}
		char buffer[64];
		if (lifetime > 0.01f && !Bomb->IsBombDefused())
		{
			sprintf_s(buffer, "Bomb: %.1f", lifetime);
			Render::Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), Render::Fonts::ESP, buffer);
		}

	}

	g_Engine->GetScreenSize(xd, meme);
	int halfX = xd / 2;
	int halfY = meme / 2;


	if (Bomb->GetBombDefuser() > 0)
	{
		float countdown = Bomb->GetC4DefuseCountDown() - (pLocal->GetTickBase() * g_Globals->interval_per_tick);
		if (countdown > 0.01f)
		{
			if (lifetime > countdown)
			{
				char defuseTimeString[24];
				sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
				Render::Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), Render::Fonts::Defuse, defuseTimeString);
			}
			else
			{
				Render::Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), Render::Fonts::Defuse, "Nu time! Git gud!");
			}
		}
	}
}

void IESP::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	auto parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (g_EntityList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}
	if (menu.Visuals.C4World)
	{
		if (adopted)
		{
			if (Render::WorldToScreen(vOrig, vScreen))
			{
				Render::Text((int)vScreen.x, (int)vScreen.y, Color(112, 20, 20, 255), Render::Fonts::ESP, "Bomb");
			}
		}
	}
}

void IESP::DrawBox(IESP::ESPBox size, Color color)
{
	Render::Outline(size.x, size.y, size.w, size.h, color);
	Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150));
	Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
}
bool IESP::GetBox(IClientEntity* pEntity, IESP::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = (int)left;
	result.y = (int)top;
	result.w = int(right - left);
	result.h = int(bottom - top);
	result.gay = (int)top;

	return true;
}
void IESP::BoxAndText(IClientEntity* pEntity, std::string text)
{
	ESPBox Box;
	std::vector<std::string> Info;
	if (GetBox(pEntity, Box))
	{
		Info.push_back(text);
		if (menu.Visuals.GrenadeESP)
		{
			DrawBox(Box, Color(255, 255, 255, 255));
			int i = 0;
			for (auto kek : Info)
			{
				Render::Text(Box.x + 1, Box.y + 1, Color(255, 255, 255, 255), Render::Fonts::ESP, kek.c_str());
				i++;
			}
		}
	}
}
void IESP::DrawThrowable(IClientEntity* throwable)
{
	model_t* nadeModel = (model_t*)throwable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = g_ModelInfo->GetStudiomodel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, "thrown") && !strstr(hdr->name, "dropped"))
		return;

	std::string nadeName = "Unknown Grenade";

	IMaterial* mats[32];
	g_ModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), "flashbang"))
		{
			nadeName = "Flashbang";
			break;
		}
		else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
		{
			nadeName = "HE";
			break;
		}
		else if (strstr(mat->GetName(), "smoke"))
		{
			nadeName = "Smoke";
			break;
		}
		else if (strstr(mat->GetName(), "decoy"))
		{
			nadeName = "Decoy";
			break;
		}
		else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
		{
			nadeName = "Molotov";
			break;
		}
	}

	BoxAndText(throwable, nadeName);
}


void IESP::DrawScoreboard()
{
	if (menu.Misc.customtab == 0)
		return;

	static bool shouldDraw = false;

	if (G::PressedKeys[menu.Misc.customtab])
	{
		shouldDraw = true;
	}
	else
	{
		shouldDraw = false;
	}

	if (!shouldDraw)
		return;

	int boardWidth = 950, boardHeight = 325;

	g_Engine->GetScreenSize(width, height);
	int x = width / 2 - boardWidth / 2;
	int y = height / 2 - boardHeight / 2;

	Render::Clear(x, y, boardWidth, boardHeight, Color(10, 10, 10));
	Render::Text( x + 10, y + 4, Color(255, 255, 255), Render::Fonts::ESP, ("pasteHOOK scoreboard"));
	Render::Text( x + 375 + 65, y + 4, Color(255, 255, 255), Render::Fonts::ESP, ("Money"));
	Render::Text( x + 475 + 65, y + 4, Color(255, 255, 255), Render::Fonts::ESP, ("Wins"));
	Render::Text( x + 605, y + 4, Color(255, 255, 255), Render::Fonts::ESP, ("Rank"));
	Render::Text( x + 805, y + 4, Color(255, 255, 255), Render::Fonts::ESP, ("Steam ID"));
	Render::Line(x + 10, y + 26, x + boardWidth - 10, y + 26, Color(90, 90, 90, 205));
	Render::DrawOutlinedRect(x - 2, y - 2, boardWidth + 4, boardHeight + 4, Color(0, 0, 0));

	Render::Text( x + 15, y + 35, Color(255, 0, 0), Render::Fonts::ESP, ("Terrorists"));
	Render::Line(x + 10, y + 50, x + boardWidth - 10, y + 50, Color(255, 0, 0));

	Render::Text(x + 15, y + 190, Color(0, 100, 255), Render::Fonts::ESP, ("Counter-Terrorists"));
	Render::Line(x + 10, y + 205, x + boardWidth - 10, y + 205, Color(0, 100, 255));

	if(playerresources == nullptr)
		playerresources = **(C_CSPlayerResource***)(U::FindPattern("client.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 2);

	if (!playerresources)
		return;



	int place = 0, place2 = 0;
	bool doSwap = false, doSwap2 = false;

	for (int i = 1; i <= g_Globals->maxClients; i++)
	{
		player_info_t playerInfo;

		IClientEntity *entity = g_EntityList->GetClientEntity(i);
		if (!entity
			|| !g_Engine->GetPlayerInfo(i, &playerInfo))
			continue;

		int playerRank = *reinterpret_cast< int * >(playerresources + offsetz.DT_CSPlayerResource.m_iCompetitiveRanking + i * 4);
		int playerWins = *reinterpret_cast< int * >(playerresources + offsetz.DT_CSPlayerResource.m_iCompetitiveWins + i * 4);

		char playerRankName[64];
		switch (playerRank)
		{
		case 0: { strcpy_s(playerRankName, ("Unranked")); break; }
		case 1: { strcpy_s(playerRankName, ("Silver I")); break; }
		case 2: { strcpy_s(playerRankName, ("Silver II")); break; }
		case 3: { strcpy_s(playerRankName, ("Silver III")); break; }
		case 4: { strcpy_s(playerRankName, ("Silver IV")); break; }
		case 5: { strcpy_s(playerRankName, ("Silver Elite")); break; }
		case 6: { strcpy_s(playerRankName, ("Silver Elite Master")); break; }
		case 7: { strcpy_s(playerRankName, ("Gold Nova I")); break; }
		case 8: { strcpy_s(playerRankName, ("Gold Nova II")); break; }
		case 9: { strcpy_s(playerRankName, ("Gold Nova III")); break; }
		case 10: { strcpy_s(playerRankName, ("Gold Nova Master")); break; }
		case 11: { strcpy_s(playerRankName, ("Master Guardian I")); break; }
		case 12: { strcpy_s(playerRankName, ("Master Guardian II")); break; }
		case 13: { strcpy_s(playerRankName, ("Master Guardian Elite")); break; }
		case 14: { strcpy_s(playerRankName, ("Distinguished Master Guardian")); break; }
		case 15: { strcpy_s(playerRankName, ("Legendary Eagle")); break; }
		case 16: { strcpy_s(playerRankName, ("Legendary Eagle Master")); break; }
		case 17: { strcpy_s(playerRankName, ("Supreme Master First Class")); break; }
		case 18: { strcpy_s(playerRankName, ("The Global Elite")); break; }
		}

		if (entity->GetTeamNum() == 2)
		{
			place++;

			if (place <= 5)
			{
				doSwap = !doSwap;

				players[i].name = playerInfo.name;
				players[i].steamid = playerInfo.guid;
				players[i].money = entity->GetMoney();
				players[i].mmrank = playerRankName;
				players[i].mmwins = playerWins;

				if (doSwap)
					Render::Clear(x + 8, y + 38 + (place * 20), boardWidth - 18, 19, Color(20, 20, 20, 150));

				Render::Text(x + 10, y + 40 + (place * 20), Color(255, 0, 0), Render::Fonts::ESP, players[i].name);
				Render::Textf( x + 375 + 65, y + 40 + (place * 20), Color(255, 0, 0), Render::Fonts::ESP, "$%i", players[i].money);
				Render::Textf( x + 475 + 65, y + 40 + (place * 20), Color(255, 0, 0), Render::Fonts::ESP, "%i", players[i].mmwins);
				Render::Text( x + 605, y + 40 + (place * 20), Color(255, 0, 0), Render::Fonts::ESP, players[i].mmrank);
				Render::Text( x + 805, y + 40 + (place * 20), Color(255, 0, 0), Render::Fonts::ESP, players[i].steamid);
			}
		}

		if (entity->GetTeamNum() == 3)
		{
			place2++;

			if (place2 <= 5)
			{
				doSwap2 = !doSwap2;

				players[i].name = playerInfo.name;
				players[i].steamid = playerInfo.guid;
				players[i].money = entity->GetMoney();
				players[i].mmrank = playerRankName;
				players[i].mmwins = playerWins;

				if (doSwap2)
					Render::Clear(x + 8, y + 193 + (place2 * 20), boardWidth - 18, 19, Color(20, 20, 20, 150));

				Render::Text(x + 10, y + 195 + (place2 * 20), Color(0, 100, 255), Render::Fonts::ESP, players[i].name);
				Render::Textf(x + 375 + 65, y + 195 + (place2 * 20), Color(0, 100, 255), Render::Fonts::ESP, "$%i", players[i].money);
				Render::Textf(x + 475 + 65, y + 195 + (place2 * 20), Color(0, 100, 255), Render::Fonts::ESP, "%i", players[i].mmwins);
				Render::Text(x + 605, y + 195 + (place2 * 20), Color(0, 100, 255), Render::Fonts::ESP, players[i].mmrank);
				Render::Text(x + 805, y + 195 + (place2 * 20), Color(0, 100, 255), Render::Fonts::ESP, players[i].steamid);
			}
		}
	}

	place = 0, place2 = 0;
	doSwap = false, doSwap2 = false;
}