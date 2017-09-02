
#include "LegitBot.h"
#include "RenderManager.h"
#include "SDK.h"
#include "EnginePrediction.h"
#include "Global.h"
#define NOMINMAX
#include <Windows.h>
#include <stdio.h>
#include <random>
#include <string>
#include <vector>
#define M_RADPI 57.295779513082f
void ILegit::Init()
{
    best_target = -1;

    view_angle = QAngle(0.0f, 0.0f, 0.0f);
    aim_angle = QAngle(0.0f, 0.0f, 0.0f);
    delta_angle = QAngle(0.0f, 0.0f, 0.0f);
    final_angle = QAngle(0.0f, 0.0f, 0.0f);

    hitbox_position = Vector(0.0f, 0.0f, 0.0f);

    aim_key = 0;
    aim_smooth = 1;
    aim_fov = 0;
    randomized_smooth = 0;
    recoil_min = 0;
    recoil_max = 0;
    randomized_angle = 0;
    shoot = false;
}
float get_fov(const QAngle &viewAngles, const QAngle &aimAngles)
{
    Vector ang, aim;
    AngleVectors(viewAngles, &aim);
    AngleVectors(aimAngles, &ang);

    return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}
QAngle compute_angle(const Vector &source, const Vector &destination)
{
    QAngle angles;

    Vector delta = source - destination;
    angles.x = static_cast< float >(asin(delta.z / delta.Length()) * M_RADPI);
    angles.y = static_cast< float >(atan(delta.y / delta.x) * M_RADPI);
    angles.z = 0.0f;

    if (delta.x >= 0.0f)
        angles.y += 180.0f;

    return angles;
}
float random_number_range(float min, float max)
{
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_real_distribution<> distribution(min, max);
    return static_cast< float >(distribution(engine));
}
void ILegit::PaintTraverse()
{

}
static size_t topkekd = GetTickCount() + 2000;
bool shoot;
static int custom_delay = 0;

void ILegit::CreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket)
{

	if (!menu.Legitbot.b1g)
		return;
	IClientEntity *pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
    CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	
	if (pLocal && pLocal->IsAlive() && pWeapon)
	{
		

        do_aimbot(pLocal, pWeapon, pCmd);

		if (!G::PressedKeys[menu.Legitbot.Triggerbot.Key]) custom_delay = 0;

		if (menu.Legitbot.Triggerbot.Enabled && menu.Legitbot.Triggerbot.Key != 0 && G::PressedKeys[menu.Legitbot.Triggerbot.Key])
            triggerbot(pCmd, pLocal, pWeapon);

		
	}

}

void ILegit::triggerbot(CInput::CUserCmd *cmd, IClientEntity* local, CBaseCombatWeapon* weapon)
{
    if (!local->IsAlive())
        return;

    if (weapon) {
        if (weapon->GetAmmoInClip() == 0)
            return;
        if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon)) return;
        if (*weapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64) return;
    }



    QAngle ViewAngles = cmd->viewangles + local->localPlayerExclusive()->GetAimPunchAngle() ;


    Vector CrosshairForward;
    AngleVectors(ViewAngles, &CrosshairForward);
    //CrosshairForward *= LocalPlayerWeaponData->m_flRange;
    CrosshairForward *= 8000.f;


    Vector TraceSource = local->GetEyePosition() ;
    Vector TraceDestination = TraceSource + CrosshairForward;

    Ray_t Ray;
    trace_t Trace;
    CTraceFilter Filter;

    Filter.pSkip = local;

    Ray.Init(TraceSource, TraceDestination);
    I::Trace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

    if (!Trace.m_pEnt)
        return;
    if (!Trace.m_pEnt->IsAlive())
        return;
    if (Trace.m_pEnt->GetHealth() <= 0 || Trace.m_pEnt->GetHealth() > 100)
        return;
    if (Trace.m_pEnt->HasGunGameImmunity())
        return;


    if (local->GetTeamNum() == Trace.m_pEnt->GetTeamNum())
        return;

    if (!hit_chance(local, cmd, weapon, Trace.m_pEnt))
        return;

    bool didHit = false;
    if ((menu.Legitbot.Triggerbot.Filter.Head && Trace.hitgroup == 1)
        || (menu.Legitbot.Triggerbot.Filter.Chest && Trace.hitgroup == 2)
        || (menu.Legitbot.Triggerbot.Filter.Stomach && Trace.hitgroup == 3)
        || (menu.Legitbot.Triggerbot.Filter.Arms && (Trace.hitgroup == 4 || Trace.hitgroup == 5))
        || (menu.Legitbot.Triggerbot.Filter.Legs && (Trace.hitgroup == 6 || Trace.hitgroup == 7)))
    {
        didHit = true;
    }

    if (menu.Legitbot.Triggerbot.Delay >= 1)
    {
        if (custom_delay >= menu.Legitbot.Triggerbot.Delay / 30)
        {
            if (didHit)
            {
                custom_delay = 0;
                shoot = true;
                cmd->buttons |= IN_ATTACK;
            }
        }
        else
        {
            custom_delay++;
        }
    }

}
void compute_angle(const Vector &source, const Vector &destination, QAngle& angles)
{
    Vector delta = source - destination;
    angles.x = static_cast< float >(asin(delta.z / delta.Length()) * M_RADPI);
    angles.y = static_cast< float >(atan(delta.y / delta.x) * M_RADPI);
    angles.z = 0.0f;

    if (delta.x >= 0.0f)
        angles.y += 180.0f;
}
float get_distance(const Vector &start, const Vector &end)
{
    float distance = sqrt((start - end).Length());

    if (distance < 1.0f)
        distance = 1.0f;

    return distance;
}
void clamp_angles(QAngle& angles)
{
    if (angles.x > 89.0f) angles.x = 89.0f;
    else if (angles.x < -89.0f) angles.x = -89.0f;

    if (angles.y > 180.0f) angles.y = 180.0f;
    else if (angles.y < -180.0f) angles.y = -180.0f;

    angles.z = 0;
}
bool sanitize_angles(QAngle &angles)
{
    QAngle temp = angles;
    MiscFunctions::NormaliseViewAngle(temp);
    clamp_angles(temp);

    if (!isfinite(temp.x) ||
        !isfinite(temp.y) ||
        !isfinite(temp.z))
        return false;

    angles = temp;

    return true;
}
void ILegit::do_aimbot(IClientEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd)
{
    if (!menu.Legitbot.b1g)
        return;

    if (!weapon)
        return;

    if (!local)
        return;
    if (!cmd)
        return;

    if (!local->IsAlive())
        return;

    if (!weapon->GetAmmoInClip() > 0)
        return;


    weapon_settings(weapon);

    if (!aim_key)
        return;

    if (!G::PressedKeys[aim_key])
        return;



    best_target = get_target(local, weapon, cmd, hitbox_position);


    if (best_target == -1)
        return;
    IClientEntity* entity = (IClientEntity*)I::EntityList->GetClientEntity(best_target);
    if (!entity)
        return;



    if (get_distance(local->GetEyePosition(), hitbox_position) > 8192.0f)
        return;


    compute_angle(local->GetEyePosition(), hitbox_position, aim_angle);
    MiscFunctions::NormaliseViewAngle(aim_angle);

    if (hitbox_position == Vector(0, 0, 0))
        return;

    aim_angle -= get_randomized_recoil(local);
    aim_angle += get_randomized_angles(local);

    MiscFunctions::NormaliseViewAngle(view_angle);

    delta_angle = view_angle - aim_angle;
    MiscFunctions::NormaliseViewAngle(delta_angle);

    float randomSmoothing = 1.0f;

    if (randomized_smooth > 1.0f)
        randomSmoothing = random_number_range(randomized_smooth / 10.0f, 1.0f);

    final_angle = view_angle - delta_angle / aim_smooth * randomSmoothing;
    MiscFunctions::NormaliseViewAngle(final_angle);

    if (!sanitize_angles(final_angle))
        return;

    cmd->viewangles = final_angle;
    I::Engine->SetViewAngles(cmd->viewangles);
}

bool ILegit::hit_chance(IClientEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, IClientEntity* target)
{
    Vector forward, right, up;

    constexpr auto max_traces = 150;

    AngleVectors(cmd->viewangles, &forward, &right, &up);

    int total_hits = 0;
    int needed_hits = static_cast<int>(max_traces * (menu.Legitbot.Triggerbot.hitchance / 100.f));

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

        QAngle viewAnglesSpread;
        VectorAngles(viewSpreadForward, viewAnglesSpread);
        MiscFunctions::NormaliseViewAngle(viewAnglesSpread);
        clamp_angles(viewAnglesSpread);

        Vector viewForward;
        AngleVectors(viewAnglesSpread, &viewForward);
        viewForward.NormalizeInPlace();

        viewForward = eyes + (viewForward * flRange);

        trace_t tr;
        Ray_t ray;
        ray.Init(eyes, viewForward);

        I::Trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);


        if (tr.m_pEnt == target)
            total_hits++;

        if (total_hits >= needed_hits)
            return true;

        if ((max_traces - i + total_hits) < needed_hits)
            return false;
    }

    return false;
}

void ILegit::weapon_settings(CBaseCombatWeapon* weapon)
{
    if (!weapon)
        return;


    if (MiscFunctions::IsSniper(weapon))
    {
        aim_key = menu.Legitbot.SniperKey;
        aim_smooth = menu.Legitbot.SniperSmooth;
        aim_fov = menu.Legitbot.Sniperfov;
        randomized_smooth = 1;
        recoil_min = menu.Legitbot.sniper_recoil_min;
        recoil_max = menu.Legitbot.sniper_recoil_max;
        randomized_angle = 1;

    }
    else if (MiscFunctions::IsPistol(weapon))
    {
        aim_key = menu.Legitbot.PistolKey;
        aim_smooth = menu.Legitbot.PistolSmooth;
        aim_fov = menu.Legitbot.Pistolfov;
        randomized_smooth = 1;
        recoil_min = menu.Legitbot.pistol_recoil_min;
        recoil_max = menu.Legitbot.pistol_recoil_max;
        randomized_angle = 1;
    }
    else
    {
        aim_key = menu.Legitbot.MainKey;
        aim_smooth = menu.Legitbot.MainSmooth;
        aim_fov = menu.Legitbot.Mainfov;
        randomized_smooth = 1;
        recoil_min = menu.Legitbot.main_recoil_min;
        recoil_max = menu.Legitbot.main_recoil_max;
        randomized_angle = 1;
    }
}

QAngle ILegit::get_randomized_recoil(IClientEntity *local)
{
    QAngle compensatedAngles = (local->localPlayerExclusive()->GetAimPunchAngle() * 2.0f) * (random_number_range(recoil_min, recoil_max) / 100.0f);
    MiscFunctions::NormaliseViewAngle(compensatedAngles);

    return (local->m_iShotsFired() > 1 ? compensatedAngles : QAngle(0.0f, 0.0f, 0.0f));
}

QAngle ILegit::get_randomized_angles(IClientEntity *local)
{
    QAngle randomizedValue = QAngle(0.0f, 0.0f, 0.0f);

    float randomRate = random_number_range(-randomized_angle, randomized_angle);
    float randomDeviation = random_number_range(-randomized_angle, randomized_angle);

    switch (rand() % 2)
    {
    case 0:
        randomizedValue.x = (randomRate * cos(randomDeviation));
        randomizedValue.y = (randomRate * cos(randomDeviation));
        randomizedValue.z = (randomRate * cos(randomDeviation));
        break;
    case 1:
        randomizedValue.x = (randomRate * sin(randomDeviation));
        randomizedValue.y = (randomRate * sin(randomDeviation));
        randomizedValue.z = (randomRate * sin(randomDeviation));
        break;
    }

    MiscFunctions::NormaliseViewAngle(randomizedValue);

    return (local->m_iShotsFired() > 1 ? randomizedValue : QAngle(0.0f, 0.0f, 0.0f));
}
bool get_hitbox_pos(IClientEntity* entity, int hitbox, Vector &output)
{
    if (hitbox >= 20)
        return false;

    const model_t *model = entity->GetModel();
    if (!model)
        return false;

    studiohdr_t *studioHdr = I::ModelInfo->GetStudiomodel(model);
    if (!studioHdr)
        return false;

    matrix3x4 matrix[128];
    if (!entity->SetupBones(matrix, 128, 0x100, 0))
        return false;

    mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
    if (!studioBox)
        return false;

    Vector min, max;

    VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
    VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

    output = (min + max) * 0.5f;

    return true;
}
bool ILegit::get_hitbox(IClientEntity *local, IClientEntity *entity, Vector &destination)
{
    int bestHitbox = -1;
    float best_fov = aim_fov;

    static const std::vector<int> hitboxes = { (int)CSGOHitboxID::Head };

    for (auto hitbox : hitboxes)
    {
        Vector temp;
        if (!get_hitbox_pos(entity, hitbox, temp))
            continue;

        float fov = get_fov(view_angle, compute_angle(local->GetEyePosition(), temp));
        if (fov < best_fov)
        {
            best_fov = fov;
            bestHitbox = hitbox;
        }
    }

    if (bestHitbox != -1)
    {
        if (!get_hitbox_pos(entity, bestHitbox, destination))
            return true;
    }

    return false;
}


int ILegit::get_target(IClientEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd, Vector &destination)
{
    int best_target = -1;
    float best_fov = aim_fov;

    I::Engine->GetViewAngles(view_angle);

    for (int i = 1; i <= I::Globals->maxClients; i++)
    {
        IClientEntity *entity = (IClientEntity*)I::EntityList->GetClientEntity(i);
        if (!entity
            || entity == local
            || entity->IsDormant()
            || entity->GetLifeState() != LIFE_ALIVE
            || entity->HasGunGameImmunity()
            || entity->GetClientClass()->m_ClassID != (int)ClassID::CCSPlayer
            || entity->GetTeamNum() == local->GetTeamNum()
            || !(entity->GetFlags() & FL_ONGROUND))
            continue;

        Vector hitbox;
        if (get_hitbox(local, entity, hitbox))
            continue;

        float fov = get_fov(view_angle + (local->localPlayerExclusive()->GetAimPunchAngle() * 2.0f), compute_angle(local->GetEyePosition(), hitbox));
        if (fov < best_fov && fov < aim_fov)
        {
            if (MiscFunctions::IsVisible(local, entity, 0))
            {
                best_fov = fov;
                destination = hitbox;
                best_target = i;
            }
        }
    }

    return best_target;
}