

#pragma once

#include "entities.h"
#include "Hacks.h"
#include "SDK.h"

Vector GetAutostrafeView();

class IMisc : public CHack
{
public:
	void Init();
	void PaintTraverse();
	void CreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket);
	void OpenConsole(CInput::CUserCmd *pCmd);
private:

	void AutoJump(CInput::CUserCmd *pCmd);
	void RageAutoStrafe(CInput::CUserCmd* pCmd);
	void pAutoStrafe(CInput::CUserCmd* pCmd);
	void AutoDefuse(CInput::CUserCmd* pCmd, IClientEntity* pEntity, ClientClass* cClass);
	void AutoStrafe(CInput::CUserCmd *pCmd);
	void ChatSpamRegular();
	void ChatSpamSlow();
	void CustomClanTag();
	void iwebz();
	void aimware();
	void kapiSware();
	void unity();
	void nevervac();
	void AirStuck(CInput::CUserCmd *pCmd);
	void NoName();
	void FPSBooster();
	void NameChangerFix();
	void ClanTagChanger();
	void memehentai();
	void ChatSpam();
	void NameStealer();
};