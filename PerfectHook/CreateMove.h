#pragma once
#include "HookIncludes.h"
#include "MiscHacks.h"
#include "MovementRecorder.h"
#include "RageBot.h"
#include "GrenadePrediction.h"
#include "LagComp.h"

using create_move_t = void(__thiscall *)(IBaseClientDLL *, int, float, bool);




int m_heldMemes;
QAngle qLastTickAngles = QAngle(0.0f, 0.0f, 0.0f);



void __stdcall CHLCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
    static auto ofunc = hooks::client.get_original<create_move_t>(21);
    ofunc(g_Client, sequence_number, input_sample_frametime, active);
	IClientEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CInput::CUserCmd *cmd = g_Input->GetUserCmd(0, sequence_number);
	if (!cmd)
		return;

	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)(reinterpret_cast< uint32_t >(g_Input) + 0xF0);
	CInput::CVerifiedUserCmd *verified = &verifiedCommands[sequence_number % 150];
	if (!verified)
		return;


	Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
	Vector qAimAngles;
	qAimAngles.Init(0.0f, cmd->viewangles.y, 0.0f);

	AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

    legitLagComp(cmd, pLocal);

	Hacks::MoveHacks(cmd, bSendPacket);
	if (cmd->buttons & IN_SCORE)
	{
        static auto ServerRankRevealAll = (bool(__cdecl*)(int*))((PDWORD)U::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x8B\x0D\x00\x00\x00\x00\x68\x00\x00\x00\x00", "xxxxx????x????"));
        static int fArray[3] = { 0,0,0 };
        ServerRankRevealAll(fArray);
	}
    grenade_prediction::instance().Tick(cmd->buttons);


	qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f);
	AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
	qAimAngles.Init(0.0f, cmd->viewangles.y, 0.0f);
	AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
	Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
	Vector vRightNorm;			Normalize(viewright, vRightNorm);
	Vector vUpNorm;				Normalize(viewup, vUpNorm);

	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;

	if (forward > 450) forward = 450;
	if (right > 450) right = 450;
	if (up > 450) up = 450;
	if (forward < -450) forward = -450;
	if (right < -450) right = -450;
	if (up < -450) up = -450;

	cmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
	cmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
	cmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);


    sanitize_angles(cmd->viewangles);
    if (!bSendPacket && menu.Ragebot.YawFake)
    {
        qLastTickAngles = cmd->viewangles;
    }
    else if(!menu.Ragebot.YawFake && bSendPacket) qLastTickAngles = cmd->viewangles;
        
    if (!sanitize_angles(cmd->viewangles))
        return;

    if (cmd->forwardmove >= 450) cmd->forwardmove = 450;
    if (cmd->sidemove >= 450) cmd->sidemove = 450;
    if (cmd->upmove >= 450) cmd->upmove = 450;
    if (cmd->forwardmove <= -450) cmd->forwardmove = -450;
    if (cmd->sidemove <= -450) cmd->sidemove = -450;
    if (cmd->upmove <= -450) cmd->upmove = -450; 

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}
#pragma warning(disable : 4409)
__declspec(naked) void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		PUSH	EBP
		MOV		EBP, ESP
		PUSH	EBX
		LEA		ECX, [ESP]
		PUSH	ECX
		PUSH	active
		PUSH	input_sample_frametime
		PUSH	sequence_number
		CALL	CHLCreateMove
		POP		EBX
		POP		EBP
		RETN	0xC
	}
}