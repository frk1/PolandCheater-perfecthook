


#pragma once

#include "Hacks.h"

class ILegit : public CHack
{
public:
	void Init();
	void PaintTraverse();
	void CreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket);
private:
	// Targetting

    void weapon_settings(CBaseCombatWeapon* weapon);
    bool hit_chance(IClientEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, IClientEntity* target);

    void do_aimbot(IClientEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd);
    QAngle get_randomized_recoil(IClientEntity* local);
    QAngle get_randomized_angles(IClientEntity* local);
    void triggerbot(CInput::CUserCmd* cmd, IClientEntity* local, CBaseCombatWeapon* weapon);

	// Functionality
    bool get_hitbox(IClientEntity *local, IClientEntity *entity, Vector &destination);
    int get_target(IClientEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd, Vector &destination);

	// Aimbot
    int best_target;

    QAngle view_angle;
    QAngle aim_angle;
    QAngle delta_angle;
    QAngle final_angle;

    Vector hitbox_position;

    int aim_key;
    float aim_smooth;
    float aim_fov;
    float randomized_smooth;
    float recoil_min;
    float recoil_max;
    float randomized_angle;
    bool shoot;


};