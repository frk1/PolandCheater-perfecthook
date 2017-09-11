
#include "RageBot.h"
#include "RenderManager.h"
#include "Autowall.h"
#include <iostream>
#include "MathFunctions.h"
#include "SDK.h"
#include "EnginePrediction.h"
#include "LagComp.h"

using namespace std;

#define TICK_INTERVAL			(g_Globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

ragebot::ragebot()
{
	IsLocked = false;
	TargetID = -1;
	pTarget = nullptr;
    AimPoint = Vector(0,0,0);
    HitBox = -1;
}





void ragebot::OnCreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket, IClientEntity* local)
{
    if (!menu.Ragebot.MainSwitch)
        return;

    if (local && local->IsAlive())
    {
        if (menu.Ragebot.BAIMkey && G::PressedKeys[menu.Ragebot.BAIMkey] && menu.Ragebot.Hitscan != 4)
        {
            menu.Ragebot.Hitscan = 4;
        }
        else if (menu.Ragebot.BAIMkey && !G::PressedKeys[menu.Ragebot.BAIMkey] && menu.Ragebot.Hitscan != 3)
        {
            menu.Ragebot.Hitscan = 3;
        }

        CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
        if (weapon && weapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex() == 64)
        {
            if (!CanAttack() && weapon->GetAmmoInClip() > 0)
            {
                pCmd->buttons |= IN_ATTACK;
            }
        }


        if (menu.Ragebot.Enabled)
            DoAimbot(pCmd, bSendPacket, local);


        if (menu.Ragebot.EnabledAntiAim)
            DoAntiAim(pCmd, bSendPacket, local);
    }
}

bool ragebot::hit_chance(IClientEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, IClientEntity* target)
{
    Vector forward, right, up;

    constexpr auto max_traces = 256;

    AngleVectors(cmd->viewangles, &forward, &right, &up);

    int total_hits = 0;
    int needed_hits = static_cast<int>(max_traces * (menu.Ragebot.HitchanceAmount / 100.f));

    weapon->UpdateAccuracyPenalty(weapon);

    auto eyes = local->GetEyePosition();
    auto flRange = weapon->GetCSWpnData()->m_fRange;

    for (int i = 0; i < max_traces; i++) {
        RandomSeed(i + 1);

        float fRand1 = RandomFloat(0.f, 1.f);
        float fRandPi1 = RandomFloat(0.f, XM_2PI);
        float fRand2 = RandomFloat(0.f, 1.f);
        float fRandPi2 = RandomFloat(0.f, XM_2PI);

        float fRandInaccuracy = fRand1 * weapon->GetInaccuracy();
        float fRandSpread = fRand2 * weapon->GetSpread();

        float fSpreadX = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
        float fSpreadY = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

        auto viewSpreadForward = (forward + fSpreadX * right + fSpreadY * up).Normalized();

        Vector viewAnglesSpread;
        VectorAngles(viewSpreadForward, viewAnglesSpread);
        sanitize_angles(viewAnglesSpread);

        Vector viewForward;
        AngleVectors(viewAnglesSpread, &viewForward);
        viewForward.NormalizeInPlace();

        viewForward = eyes + (viewForward * flRange);

        trace_t tr;
        Ray_t ray;
        ray.Init(eyes, viewForward);

        g_Trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);


        if (tr.m_pEnt == target)
            total_hits++;

        if (total_hits >= needed_hits)
            return true;

        if ((max_traces - i + total_hits) < needed_hits)
            return false;
    }

    return false;
}

void ragebot::DoAimbot(CInput::CUserCmd *pCmd, bool& bSendPacket, IClientEntity* local)
{

    bool FindNewTarget = true;
    CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());

    if (!pWeapon) return;
    if (pWeapon->GetAmmoInClip() == 0 || MiscFunctions::IsKnife(pWeapon) || MiscFunctions::IsGrenade(pWeapon))
        return;

    // Make sure we have a good target
    if (IsLocked && TargetID >= 0 && HitBox >= 0)
    {
        pTarget = g_EntityList->GetClientEntity(TargetID);
        if (pTarget  && TargetMeetsRequirements(pTarget, local))
        {
            HitBox = HitScan(pTarget);
            if (HitBox >= 0)
            {
                Vector View; g_Engine->GetViewAngles(View);
                Vector hitboxpos = GetHitboxPosition(pTarget, HitBox);

                float FoV = FovToPlayer(View, compute_angle(local->GetEyePosition(), hitboxpos));
                if (FoV < menu.Ragebot.FOV)
                    FindNewTarget = false;
            }
        }
    }



    // Find a new target, apparently we need to
    if (FindNewTarget)
    {
        TargetID = 0;
        pTarget = nullptr;
        HitBox = -1;


        TargetID = GetTargetCrosshair(local);


        // Memesj
        if (TargetID >= 0)
        {
            pTarget = g_EntityList->GetClientEntity(TargetID);
        }
    }

    if (TargetID >= 0 && pTarget)
    {
        HitBox = HitScan(pTarget);

        // Key
        if (menu.Ragebot.KeyPress)
        {
            if (menu.Ragebot.KeyPress > 0 && !G::PressedKeys[menu.Ragebot.KeyPress])
            {
                TargetID = -1;
                pTarget = nullptr;
                HitBox = -1;
                return;
            }
        }


        Vector AimPoint = GetHitboxPosition(pTarget, HitBox);





        if (AimAtPoint(local, AimPoint, pCmd))
        {
            if (menu.Ragebot.AutoFire && CanAttack() && MiscFunctions::IsSniper(pWeapon) && menu.Ragebot.AutoScope)
            {
                if (local->IsScoped()) if (!menu.Ragebot.Hitchance || hit_chance(local, pCmd, pWeapon, pTarget)) pCmd->buttons |= IN_ATTACK;
                if (!local->IsScoped()) pCmd->buttons |= IN_ATTACK2;
            }
            if (menu.Ragebot.AutoFire && CanAttack() && !(MiscFunctions::IsSniper(pWeapon)))
            {
                if (!menu.Ragebot.Hitchance || hit_chance(local, pCmd, pWeapon, pTarget)) pCmd->buttons |= IN_ATTACK;
            }
            if (menu.Ragebot.AutoFire && CanAttack() && (MiscFunctions::IsSniper(pWeapon)) && !menu.Ragebot.AutoScope)
            {
                if (!menu.Ragebot.Hitchance || hit_chance(local, pCmd, pWeapon, pTarget)) if (local->IsScoped()) pCmd->buttons |= IN_ATTACK;
            }
        }





        if (menu.Ragebot.AutoStop)
        {
            pCmd->forwardmove = 0.f;
            pCmd->sidemove = 0.f;
        }
        if (menu.Ragebot.AutoCrouch)
        {
            pCmd->buttons |= IN_DUCK;
        }

    }
    // Auto Pistol
    static bool WasFiring = false;
    if (pWeapon != nullptr)
    {
        CSWeaponInfo* WeaponInfo = pWeapon->GetCSWpnData();
        if (MiscFunctions::IsPistol(pWeapon) && menu.Ragebot.AutoPistol && pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex() != 64)
        {
            if (pCmd->buttons & IN_ATTACK && !MiscFunctions::IsKnife(pWeapon) && !MiscFunctions::IsGrenade(pWeapon))
            {
                if (WasFiring)
                {
                    pCmd->buttons &= ~IN_ATTACK;
                }
            }

            WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
        }
    }


}



bool ragebot::TargetMeetsRequirements(IClientEntity* pEntity, IClientEntity* local)
{
    if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != local->GetIndex())
    {
        ClientClass *pClientClass = pEntity->GetClientClass();
        player_info_t pinfo;
        if (pClientClass->m_ClassID == (int)ClassID::CCSPlayer && g_Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
            if (pEntity->GetTeamNum() != local->GetTeamNum() || menu.Ragebot.FriendlyFire)
                if (!pEntity->HasGunGameImmunity())
                    return true;
    }

	return false;
}

float ragebot::FovToPlayer(const Vector &viewAngles, const Vector &aimAngles)
{
    Vector ang, aim;
    AngleVectors(viewAngles, &aim);
    AngleVectors(aimAngles, &ang);

    return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

int ragebot::GetTargetCrosshair(IClientEntity* local)
{
	int target = -1;
	float minFoV = menu.Ragebot.FOV;


	Vector View; g_Engine->GetViewAngles(View);

	for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = g_EntityList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity, local))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
                Vector hitboxpos = GetHitboxPosition(pEntity, 0);
				float fov = FovToPlayer(View, compute_angle(local->GetEyePosition(), hitboxpos));
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int ragebot::HitScan(IClientEntity* pEntity)
{
    vector<int> HitBoxesToScan{ };

    int HitScanMode = menu.Ragebot.Hitscan;

    if (HitScanMode == 0)
    {
        switch (menu.Ragebot.Hitbox)
        {
        case 0:
            HitBoxesToScan.push_back(Head);
            break;
        case 1:
            HitBoxesToScan.push_back(Neck);
            break;
        case 2:
            HitBoxesToScan.push_back(Chest);
            break;
        case 3:
            HitBoxesToScan.push_back(Stomach);
            break;
        }
    }
    else
    {
        switch (HitScanMode)
        {
        case 1:
            // low
            HitBoxesToScan.push_back(Head);
            HitBoxesToScan.push_back(Neck);
            HitBoxesToScan.push_back(UpperChest);
            HitBoxesToScan.push_back(Chest);
            HitBoxesToScan.push_back(Stomach);
            HitBoxesToScan.push_back(Pelvis);
            break;
        case 2:
            // medium
            HitBoxesToScan.push_back(Head);
            HitBoxesToScan.push_back(Neck);
            HitBoxesToScan.push_back(UpperChest);
            HitBoxesToScan.push_back(Chest);
            HitBoxesToScan.push_back(Stomach);
            HitBoxesToScan.push_back(Pelvis);
            HitBoxesToScan.push_back(LeftUpperArm);
            HitBoxesToScan.push_back(RightUpperArm);
            HitBoxesToScan.push_back(LeftThigh);
            HitBoxesToScan.push_back(RightThigh);
            break;
        case 3:
            // high
            HitBoxesToScan.push_back(Head);
            HitBoxesToScan.push_back(Neck);
            HitBoxesToScan.push_back(UpperChest);
            HitBoxesToScan.push_back(Chest);
            HitBoxesToScan.push_back(Stomach);
            HitBoxesToScan.push_back(Pelvis);
            HitBoxesToScan.push_back(LeftUpperArm);
            HitBoxesToScan.push_back(RightUpperArm);
            HitBoxesToScan.push_back(LeftThigh);
            HitBoxesToScan.push_back(RightThigh);
            HitBoxesToScan.push_back(LeftHand);
            HitBoxesToScan.push_back(RightHand);
            HitBoxesToScan.push_back(LeftFoot);
            HitBoxesToScan.push_back(RightFoot);
            HitBoxesToScan.push_back(LeftShin);
            HitBoxesToScan.push_back(RightShin);
            HitBoxesToScan.push_back(LeftLowerArm);
            HitBoxesToScan.push_back(RightLowerArm);
            break;
        case 4:
            // baim
            HitBoxesToScan.push_back(UpperChest);
            HitBoxesToScan.push_back(Chest);
            HitBoxesToScan.push_back(Stomach);
            HitBoxesToScan.push_back(Pelvis);
            HitBoxesToScan.push_back(LeftUpperArm);
            HitBoxesToScan.push_back(RightUpperArm);
            HitBoxesToScan.push_back(LeftThigh);
            HitBoxesToScan.push_back(RightThigh);
            HitBoxesToScan.push_back(LeftHand);
            HitBoxesToScan.push_back(RightHand);
            HitBoxesToScan.push_back(LeftFoot);
            HitBoxesToScan.push_back(RightFoot);
            HitBoxesToScan.push_back(LeftShin);
            HitBoxesToScan.push_back(RightShin);
            HitBoxesToScan.push_back(LeftLowerArm);
            HitBoxesToScan.push_back(RightLowerArm);
            break;
        }
    }
    static vector<int> baim{ UpperChest ,Chest ,Stomach ,Pelvis ,LeftUpperArm ,RightUpperArm ,LeftThigh,RightThigh ,LeftHand ,RightHand, LeftFoot, RightFoot, LeftShin, RightShin,LeftLowerArm,RightLowerArm };

    int bestHitbox = -1;
    float highestDamage = menu.Ragebot.MinimumDamage;
    float health = pEntity->GetHealth();
    for (auto HitBoxID : HitBoxesToScan)
    {

        Vector Point = GetHitboxPosition(pEntity, HitBoxID); //pvs fix disabled

        float damage = 0.0f;
        if (CanHit(Point, &damage))
        {
            if (damage > highestDamage || damage > health)
            {
                bestHitbox = HitBoxID;
                highestDamage = damage;
            }
        }
    }
    for (auto HitBoxID : baim)
    {

        Vector Point = GetHitboxPosition(pEntity, HitBoxID); //pvs fix disabled

        float damage = 0.0f;
        if (CanHit(Point, &damage))
        {
            if (damage > highestDamage && damage > health)
            {
                bestHitbox = HitBoxID;
                highestDamage = damage;
            }
        }
    }
    return bestHitbox;

}

bool ragebot::AimAtPoint(IClientEntity* pLocal, Vector point, CInput::CUserCmd *pCmd)
{

	if (point.Length() == 0) return false;

	Vector angles;

	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	VectorAngles(point - src, angles);
    if (!sanitize_angles(angles)) return false;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	bool can_shoot = true;
	float server_time = pLocal->GetTickBase() * g_Globals->interval_per_tick;

	if (pWeapon != nullptr)
	{
		float next_shot = pWeapon->GetNextPrimaryAttack() - server_time;
		if (next_shot > 0) {
			can_shoot = false;
		}
	}

	IsLocked = true;

    if(menu.Ragebot.AntiRecoil) 
        angles -= pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2;

	if (menu.Ragebot.Silent && can_shoot) 
        pCmd->viewangles = angles;
	

    if (!menu.Ragebot.Silent)
    {
        pCmd->viewangles = angles;
        g_Engine->SetViewAngles(pCmd->viewangles);
    }
	return true;
}




void NormalizeVector(Vector& vec) {
	for (int i = 0; i < 3; ++i) {
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}
	vec[2] = 0.f;
}


void VectorAngles2(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = vec_t(atan2(vecForward[1], vecForward[0]) * 180.f / M_PI);

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = vec_t(atan2(vecForward[2], vecView[2]) * 180.f / M_PI);
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}



bool EdgeAntiAim(IClientEntity* pLocalBaseEntity, CInput::CUserCmd* cmd, float flWall, float flCornor)
{
	Ray_t ray;
	trace_t tr;

	CTraceFilter traceFilter;
	traceFilter.pSkip = pLocalBaseEntity;

	auto bRetVal = false;
	auto vecCurPos = pLocalBaseEntity->GetEyePosition();

	for (float i = 0; i < 360; i++)
	{
		Vector vecDummy(10.f, cmd->viewangles.y, 0.f);
		vecDummy.y += i;

		NormalizeVector(vecDummy);

		Vector vecForward;
		AngleVectors2(vecDummy, vecForward);

		auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
		vecForward *= flLength;

		ray.Init(vecCurPos, (vecCurPos + vecForward));
		g_Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter *)&traceFilter, &tr);

		if (tr.fraction != 1.0f)
		{
			Vector qAngles;
			auto vecNegate = tr.plane.normal;

			vecNegate *= -1.f;
			VectorAngles2(vecNegate, qAngles);

			vecDummy.y = qAngles.y;

			NormalizeVector(vecDummy);
			trace_t leftTrace, rightTrace;

			Vector vecLeft;
			AngleVectors2(vecDummy + Vector(0.f, 30.f, 0.f), vecLeft);

			Vector vecRight;
			AngleVectors2(vecDummy - Vector(0.f, 30.f, 0.f), vecRight);

			vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
			vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

			ray.Init(vecCurPos, (vecCurPos + vecLeft));
			g_Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &leftTrace);

			ray.Init(vecCurPos, (vecCurPos + vecRight));
			g_Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &rightTrace);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				vecDummy.y -= flCornor; // left
			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				vecDummy.y += flCornor; // right			

			cmd->viewangles.y = vecDummy.y;
			cmd->viewangles.y -= flWall;
			cmd->viewangles.x = 89.0f;
			bRetVal = true;
		}
	}
	return bRetVal;
}

// AntiAim
void ragebot::DoAntiAim(CInput::CUserCmd *pCmd, bool& bSendPacket, IClientEntity* local)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());




	// If the aimbot is doing something don't do anything
	if (pCmd->buttons & IN_ATTACK && CanAttack())
		return;
	if ((pCmd->buttons & IN_USE))
		return;
	if (local->GetMoveType() == MOVETYPE_LADDER)
		return;
	// Weapon shit

	if (pWeapon)
	{
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();
		CCSGrenade* csGrenade = (CCSGrenade*)pWeapon;


		if (MiscFunctions::IsKnife(pWeapon) && !menu.Ragebot.KnifeAA)
			return;

		if (csGrenade->GetThrowTime() > 0.f)
			return;
	}

	// Don't do antiaim
	// if (DoExit) return;

	if (menu.Ragebot.Edge) {
		auto bEdge = EdgeAntiAim(local, pCmd, 360.f, 89.f);
		if (bEdge)
			return;
	}


	

	// Anti-Aim Pitch


	//Anti-Aim Yaw
/*	switch (Menu::Window.Rage.AntiAimYaw.GetIndex())
	{
	case 0:
		// No Yaw AA
		break;
	case 1:
		// Fake sideways
		AntiAims::FakeSideways(pCmd, bSendPacket);
		break;
	case 2:
		// Slow Spin
		AntiAims::SlowSpin(pCmd);
		break;
	case 3:
		// Fast Spin
		AntiAims::FastSpin(pCmd);
		break;
	case 4:
		//backwards
		pCmd->viewangles.y -= 180;
		break;
	}*/

	static bool ySwitch;

	if (menu.Ragebot.YawFake != 0)
		ySwitch = !ySwitch;
	else
		ySwitch = true;

	bSendPacket = ySwitch;

	Vector SpinAngles;
	Vector FakeAngles;
    float server_time = local->GetTickBase() * g_Globals->interval_per_tick;
	static int ticks;
	static bool flip;
	if (ticks < 15 + rand() % 20)
		ticks++;
	else
	{
		flip = !flip;
		ticks = 0;
	}
	Vector StartAngles;
	double rate = 360.0 / 1.618033988749895;
	double yaw = fmod(static_cast<double>(server_time)*rate, 360.0);
	double factor = 360.0 / M_PI;
	factor *= 25;
	switch (menu.Ragebot.YawTrue)
	{
	case 1: //sideways
	{
		g_Engine->GetViewAngles(StartAngles);
		SpinAngles.y = flip ? StartAngles.y - 90.f : StartAngles.y + 90.f;
	}
		break;
	case 2://slowspin
		SpinAngles.y += static_cast<float>(yaw);
		break;
	case 3://fastspin
	{
		SpinAngles.y = (float)(fmod(server_time / 0.05f * 360.0f, 360.0f));
	}
		break;
	case 4://backwards
	{
		g_Engine->GetViewAngles(StartAngles);
		StartAngles.y -= 180.f;
		SpinAngles = StartAngles;
	}
		break;
	case 5:
	{
		SpinAngles.y = local->GetLowerBodyYaw();
	}
		break;
	}



	switch (menu.Ragebot.YawFake)
	{
	case 1://sideways
	{
		g_Engine->GetViewAngles(StartAngles);
		FakeAngles.y = flip ? StartAngles.y + 90.f : StartAngles.y - 90.f;
	}
		break;
	case 2://slowspin
		FakeAngles.y += static_cast<float>(yaw);
		break;
	case 3://fastspin
		FakeAngles.y = (float)(fmod(server_time / 0.05f * 360.0f, 360.0f));
	break;
	case 4://backwards
	{
		g_Engine->GetViewAngles(StartAngles);
		StartAngles -= 180.f;
		FakeAngles = StartAngles;
	}
	break;
	case 5: //lby antiaim
		{
			g_Engine->GetViewAngles(StartAngles);
			static bool llamaflip;
			static float oldLBY = 0.0f;
			float LBY = local->GetLowerBodyYaw();
			if (LBY != oldLBY) // did lowerbody update?
			{
				llamaflip = !llamaflip;
				oldLBY = LBY;
			}
			FakeAngles.y = llamaflip ? StartAngles.y + 90.f : StartAngles.y - 90.f;
		}
		break;
	}


	{
		if (ySwitch && menu.Ragebot.YawTrue != 0)
			pCmd->viewangles = SpinAngles;
		else if (!ySwitch && menu.Ragebot.YawFake != 0)
			pCmd->viewangles = FakeAngles;
	}

	switch (menu.Ragebot.Pitch)
	{
	case 0:
		// No Pitch AA
		break;
	case 1:
		// Down
		pCmd->viewangles.x = 89;
		break;
	case 2:
		pCmd->viewangles.x = -89;
		break;
	}

}


