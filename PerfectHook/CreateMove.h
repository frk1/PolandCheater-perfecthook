#pragma once
#include "HookIncludes.h"
#include "MiscHacks.h"
#include <algorithm>
#include "MovementRecorder.h"
#include "RageBot.h"
#include "GrenadePrediction.h"
backtrackData headPositions[64][16];
using CreateMoveFn = void(__thiscall *)(IBaseClientDLL *, int, float, bool);
CreateMoveFn originalCreateMove;

using tServerRankRevealAllFn = bool(__cdecl*)(int*);
static tServerRankRevealAllFn ServerRankRevealAll = (tServerRankRevealAllFn)((PDWORD)U::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x8B\x0D\x00\x00\x00\x00\x68\x00\x00\x00\x00", "xxxxx????x????"));
int fArray[3] = { 0,0,0 };
int m_heldMemes;
QAngle qLastTickAngles = QAngle(0.0f, 0.0f, 0.0f);
void ClampAngles(QAngle &angles);
void NormalizeAngles(QAngle &angles);
bool SanitizeAngles(QAngle &angles);
typedef void(__stdcall* foCreateMove)(int, float, bool);

Vector angle_vector(Vector meme)
{
    auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
    auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

    auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
    auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

    return Vector(cp*cy, cp*sy, -sp);
}
float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
    auto PointDir = Point - LineOrigin;

    auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
    if (TempOffset < 0.000001f)
        return FLT_MAX;

    auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

    return (Point - PerpendicularPoint).Length();
}
void __stdcall CHLCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	g_pClientVMTHook->GetMethod< foCreateMove >(21)(sequence_number, input_sample_frametime, active);
	//originalCreateMove(I::Client, sequence_number, input_sample_frametime, active);
	IClientEntity *pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	CInput::CUserCmd *cmd = I::Input->GetUserCmd(0, sequence_number);
	if (!cmd)
		return;

	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)(reinterpret_cast< uint32_t >(I::Input) + 0xF0);
	CInput::CVerifiedUserCmd *verified = &verifiedCommands[sequence_number % 150];
	if (!verified)
		return;

    if (G::PressedKeys[menu.Misc.ragequit] && menu.Misc.ragequit)
        DoUnload = true;
	Vector origView = cmd->viewangles;
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
	Vector qAimAngles;
	qAimAngles.Init(0.0f, cmd->viewangles.y, 0.0f);

	AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

    if (menu.Ragebot.BAIMkey && G::PressedKeys[menu.Ragebot.BAIMkey] && menu.Ragebot.Hitscan != 4)
    {
        menu.Ragebot.Hitscan = 4;
    }
    else if (menu.Ragebot.BAIMkey && !G::PressedKeys[menu.Ragebot.BAIMkey] && menu.Ragebot.Hitscan != 3)
    {
        menu.Ragebot.Hitscan = 3;
    }
    if (menu.Ragebot.Enabled && pLocal && pLocal->IsAlive())
    {
        CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
        if (weapon && weapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex() == 64)
        {
            if (!CanAttack() && weapon->GetAmmoInClip() > 0)
            {
                cmd->buttons |= IN_ATTACK;
            }
        }
    }
    if (menu.Legitbot.backtrack)
    {
        int bestTargetIndex = -1;
        float bestFov = FLT_MAX;
        player_info_t info;
        for (int i = 0; i < I::Engine->GetMaxClients(); i++)
        {            auto entity = (IClientEntity*)I::EntityList->GetClientEntity(i);

            if (!entity || !pLocal)
                continue;

            if (entity == pLocal)
                continue;

            if (!I::Engine->GetPlayerInfo(i, &info))
                continue;

            if (entity->IsDormant())
                continue;


            if (entity->GetTeamNum() == pLocal->GetTeamNum())
                continue;


            if (entity->IsAlive())
            {
                float simtime = entity->GetSimulationTime();
                Vector hitboxPos = GetHitboxPosition(entity, 0);

                headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
                Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
                float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

                if (bestFov > FOVDistance)
                {
                    bestFov = FOVDistance;
                    bestTargetIndex = i;
                }
            }
        }

        float bestTargetSimTime;
        if (bestTargetIndex != -1)
        {
            float tempFloat = FLT_MAX;
            Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
            for (int t = 0; t <= 12; ++t)
            {
                float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
                if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
                {

                    tempFloat = tempFOVDistance;
                    bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
                }
            }
            if (cmd->buttons & IN_ATTACK)
            {
                cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);

            }
        }
    }
	//if (menu.Misc.isRecording || menu.Misc.isReplaying) MovementRecorder(cmd);
	Hacks::MoveHacks(cmd, bSendPacket);
	if (cmd->buttons & IN_SCORE)
	{
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


	MiscFunctions::NormaliseViewAngle(cmd->viewangles);
	if (cmd->viewangles.z != 0.0f)
	{
		cmd->viewangles.z = 0.00;
	}
	if (cmd->viewangles.x < -89 || cmd->viewangles.x > 89 || cmd->viewangles.y < -180 || cmd->viewangles.y > 180)
	{
		MiscFunctions::NormaliseViewAngle(cmd->viewangles);
		if (cmd->viewangles.x < -89 || cmd->viewangles.x > 89 || cmd->viewangles.y < -180 || cmd->viewangles.y > 180)
		{
			cmd->viewangles = origView;
			cmd->sidemove = right;
			cmd->forwardmove = forward;
		}
	}
    if (!bSendPacket && menu.Ragebot.YawFake)
    {
        qLastTickAngles = cmd->viewangles;
    }
    else if(!menu.Ragebot.YawFake && bSendPacket) qLastTickAngles = cmd->viewangles;
        


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

void __declspec(naked) __stdcall create_move_proxy()
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx                    // Put bSendPacket (ebx register) on top of the stack
		push esp                    // Push ESP (which is now a pointer to bSendPacket)
		push[ebp + 16]             // Push 'active'
		push[ebp + 12]             // Push 'input_sample_frametime'
		push[ebp + 8]              // Push 'sequence_number'
		call CHLCreateMove     // Call our 'real' CreateMove hook which now receives bSendPacket as last parameter
		pop  ebx                    // Pop EBX that we had pushed earlier
		pop  ebp
		ret
	}
}





void ClampAngles(QAngle &angles)
{
	if (angles.x > 89.0f)
		angles.x = 89.0f;
	if (angles.x < -89.0f)
		angles.x = -89.0f;

	if (angles.y > 180.0f)
		angles.y = 180.0f;
	if (angles.y < -180.0f)
		angles.y = -180.0f;

	angles.z = 0.0f;
}

// credits: notwav
void NormalizeAngles(QAngle &angles)
{
	if (angles.x > 180.0f || angles.x < -180.0f)
	{
		float revolutions = angles.x / 360.0f;

		if (revolutions < 0.0f)
			revolutions = -revolutions;

		revolutions = round(revolutions);

		if (angles.x < 0.0f)
			angles.x = (angles.x + 360.0f * revolutions);
		else
			angles.x = (angles.x - 360.0f * revolutions);
	}

	if (angles.y > 180.0f || angles.y < -180.0f)
	{
		float revolutions = angles.y / 360.0f;

		if (revolutions < 0.0f)
			revolutions = -revolutions;

		revolutions = round(revolutions);

		if (angles.y < 0.0f)
			angles.y = (angles.y + 360.0f * revolutions);
		else
			angles.y = (angles.y - 360.0f * revolutions);
	}

	angles.z = 0.0f;
}
bool SanitizeAngles(QAngle &angles)
{
	QAngle temp = angles;
	NormalizeAngles(temp);
	ClampAngles(temp);

	if (!std::isfinite(temp.x) ||
		!std::isfinite(temp.y) ||
		!std::isfinite(temp.z))
		return false;

	angles = temp;

	return true;
}
