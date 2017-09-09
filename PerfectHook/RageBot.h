
#pragma once

#include "Hacks.h"
inline bool CanAttack()
{
    auto local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

    if (local && local->IsAlive())
    {
        CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
        if (weapon)
        {
            if (weapon->GetAmmoInClip() < 1 && !MiscFunctions::IsKnife(weapon))
                return false;

            bool revolver_can_shoot = true;
            if (weapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex() == 64)
            {
                float time_to_shoot = weapon->m_flPostponeFireReadyTime() - local->GetTickBase() * g_Globals->interval_per_tick;
                revolver_can_shoot = time_to_shoot <= g_Globals->absoluteframetime;
            }

            float time = local->GetTickBase() * g_Globals->interval_per_tick;
            float next_attack = weapon->GetNextPrimaryAttack();
            return revolver_can_shoot && !(next_attack > time);
        }
    }

    return false;
}

class IRage : public CHack
{
public:
	void Init();
	void PaintTraverse();
	bool hit_chance(IClientEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon);
	bool hit_chance(IClientEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, IClientEntity* target);
	void CreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket);

private:
	// Targetting
	int GetTargetCrosshair();
	void AngleVectors2(const Vector& qAngles, Vector& vecForward);
	int GetTargetDistance();
	int GetTargetHealth();
	bool TargetMeetsRequirements(IClientEntity* pEntity);
	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	Vector pointScanEdge(IClientEntity* pEntity, int HitboxID, float& damage);
	int HitScan(IClientEntity* pEntity);
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CInput::CUserCmd *pCmd);
	void PosAdjust(CInput::CUserCmd *pCmd);
	void AntiAfk(CInput::CUserCmd *pCmd);
	bool IsValidTARGET(int iEnt, IClientEntity* pLocal);
	int AATARGE(CInput::CUserCmd *pCmd, IClientEntity* pLocal, CBaseCombatWeapon* pWeapon);
	// Functionality
	void DoAimbot(CInput::CUserCmd *pCmd, bool& bSendPacket);
	//	void DoNoSpread(CInput::CUserCmd *pCmd);
	void DoNoRecoil(CInput::CUserCmd *pCmd);
	void DoAntiAim2(CInput::CUserCmd* Cmd, bool& bSendPacket);
	//	void Resolver(const CRecvProxyData *pData, void *pStruct, void *pOut);
	// AntiAim
	void DoAntiAim(CInput::CUserCmd *pCmd, bool& bSendPacket);
	//	void FakeLagFix();

	// AimStep
	bool IsAimStepping;
	Vector LastAimstepAngle;
	Vector LastAngle;

	// Aimbot
	bool IsLocked;
	int TargetID;
	int HitBox;
	Vector AimPoint;
	IClientEntity* pTarget;
};