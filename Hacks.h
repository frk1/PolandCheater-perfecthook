#pragma once

#pragma once

#include "CommonIncludes.h"
#include "SDK.h"
#include <vector>

namespace Hacks
{
	void SetupHacks();
	void DrawHacks();
	void MoveHacks(CInput::CUserCmd* pCmd, bool& bSendPacket);
}

class CHack
{
public:
	virtual void PaintTraverse() = 0;
	virtual void CreateMove(CInput::CUserCmd* pCmd, bool& bSendPacket) = 0;
	virtual void Init() = 0;
};

class HackManager
{
public:
	void RegisterHack(CHack* hake);
	void Ready();

	void PaintTraverse();
	void CreateMove(CInput::CUserCmd* pCmd, bool& bSendPacket);
	IClientEntity* pLocal();
private:
	std::vector<CHack*> Hacks;
	IClientEntity* pLocalInstance;
	bool IsReady;
};

extern HackManager hack;
