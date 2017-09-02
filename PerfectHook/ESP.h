

#pragma once

#include "Hacks.h"

class IESP : public CHack
{
public:
	void Init();
	void CreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket);
	void PaintTraverse();
	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo, IClientEntity* pLocal);
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

	// Draw a player
	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);
	void DrawPlayer2(IClientEntity* pEntity, player_info_t pinfo);

	// Get player info
	Color GetPlayerColor(IClientEntity* pEntity);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);
	void BoxAndText(IClientEntity* pEntity, std::string text);
	void BoxAndText(IClientEntity* pEntity, const char* text);
	void HealthBar(Vector bot, Vector top, float health);

	// Draw shit about player
	void DrawBox(ESPBox size, Color color);
	void PlayerBox(float x, float y, float w, float h, Color clr);
	void DrawName(player_info_t pinfo, ESPBox size);
	void DrawName2(player_info_t pinfo, Vector pos);
	void DrawHealth(Vector bot, Vector top, float health);
	void DrawHealth(IClientEntity* pEntity, Vector bot, Vector top);
	void DrawWeapon(player_info_t pinfo, IESP::ESPBox size);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);
	void DrawHealthBar(IClientEntity* pEntity, IESP::ESPBox size);
	void DrawGrenades(IClientEntity* pEntity);
	void DrawInfo(IClientEntity* pEntity, ESPBox size);
	void DrawInfo3(Vector bot, Vector top, IClientEntity* pEntity);
	void DrawWeapon2(IClientEntity* pEntity, IESP::ESPBox size);
	void DrawCross(IClientEntity* pEntity);
	void DrawSkeleton(IClientEntity* pEntity);

	void DrawChicken(IClientEntity* pEntity, ClientClass* cClass);


	void DrawDrop(IClientEntity* pEntity);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
	void Molotov(IClientEntity* pEntity, ClientClass* cClass, float x, float y, float w, float h);
	void Smoke(IClientEntity* pEntity, ClientClass* cClass, float x, float y, float w, float h);
	void Flash(IClientEntity* pEntity, ClientClass* cClass, float x, float y, float w, float h);
	void Decoy(IClientEntity* pEntity, ClientClass* cClass, float x, float y, float w, float h);
	void DrawGlow();
	void DrawNade2(IClientEntity* pEntity, ClientClass* cClass);
	void Molotov(IClientEntity* pEntity, ClientClass* cClass);
	void Smoke(IClientEntity* pEntity, ClientClass* cClass);
	void Flash(IClientEntity* pEntity, ClientClass* cClass);
	void Decoy(IClientEntity* pEntity, ClientClass* cClass);
	void DrawEntity(IClientEntity* entity, const char* string);
	void DrawThrowable(IClientEntity* throwable);
	void DrawScoreboard();
	void DrawScoreboard(IClientEntity* local);
	void HEGrenade(IClientEntity* pEntity, ClientClass* cClass);
};

