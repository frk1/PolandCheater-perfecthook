

#pragma once

#include "SDK.h"
#include "singleton.hpp"
class visuals
    :public singleton<visuals>
{
public:
    visuals();
	void OnPaintTraverse(IClientEntity* local);
	void DrawPlayer(IClientEntity* entity, player_info_t pinfo, IClientEntity* local);
private:
	struct playerlist_t
	{
		char *name;
		char *steamid;
		int   money;
		int   health;
		int   mmwins;
		char *mmrank;
	}players[64];
	// Other shit
	IClientEntity *BombCarrier;

	struct ESPBox
	{
		int x, y, w, h, gay;
	};
    void DLight(IClientEntity* local, IClientEntity* entity);

	// Get player info
	Color GetPlayerColor(IClientEntity* pEntity, IClientEntity* local);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);
	void BoxAndText(IClientEntity* pEntity, std::string text);

	// Draw shit about player
	void DrawBox(ESPBox size, Color color);
	void PlayerBox(float x, float y, float w, float h, Color clr);
	void DrawHealth(Vector bot, Vector top, float health);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);


	void DrawDrop(IClientEntity* pEntity);
	void DrawBombPlanted(IClientEntity* entity, IClientEntity* local);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
	void DrawThrowable(IClientEntity* throwable);
    void NightMode();
    void SpecList(IClientEntity *local);
};

inline float CSGO_Armor(float flDamage, int ArmorValue)
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

char* const itemNames[] =
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
inline float flGetDistance(Vector from, Vector to)
{
    Vector angle;
    angle.x = to.x - from.x;	angle.y = to.y - from.y;	angle.z = to.z - from.z;

    return sqrt(angle.x*angle.x + angle.y*angle.y + angle.z*angle.z);
}
