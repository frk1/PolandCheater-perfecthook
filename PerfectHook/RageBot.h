
#pragma once

#include "SDK.h"
#include "singleton.hpp"

class ragebot
    : public singleton<ragebot>
{
public:

    ragebot();

    void OnCreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket, IClientEntity* local);
	bool hit_chance(IClientEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, IClientEntity* target);
private:
	int GetTargetCrosshair(IClientEntity* local);
	bool TargetMeetsRequirements(IClientEntity* pEntity, IClientEntity* local);
    float FovToPlayer(const QAngle &viewAngles, const QAngle &aimAngles);
	int HitScan(IClientEntity* pEntity);
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CInput::CUserCmd *pCmd);
	void DoAimbot(CInput::CUserCmd *pCmd, bool& bSendPacket, IClientEntity* local);
	void DoAntiAim(CInput::CUserCmd *pCmd, bool& bSendPacket, IClientEntity* local);
private:
	bool IsLocked;
	int TargetID;
	int HitBox;
	Vector AimPoint;
	IClientEntity* pTarget;
};

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