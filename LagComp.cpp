#include "LagComp.h"

void llamaBT::Update(int tick_count)
{
    if (!menu.Ragebot.FakeLagFix)
        return;

    latest_tick = tick_count;
    for (int i = 0; i < 64; i++)
    {
        UpdateRecord(i);
    }
}

bool llamaBT::IsTickValid(int tick)
{
    int delta = latest_tick - tick;
    float deltaTime = delta * I::Globals->interval_per_tick;
    return (fabs(deltaTime) <= 0.2f);
}

void llamaBT::UpdateRecord(int i)
{
    IClientEntity* pEntity = I::EntityList->GetClientEntity(i);
    if (pEntity && pEntity->IsAlive() && !pEntity->IsDormant())
    {
        float lby = pEntity->GetLowerBodyYaw();
        if (lby != records[i].lby)
        {
            records[i].tick_count = latest_tick;
            records[i].lby = lby;
            records[i].headPosition = GetHitboxPosition(pEntity, 0);
        }
    }
    else
    {
        records[i].tick_count = 0;
    }
}

bool llamaBT::BacktrackNigger(int i, CInput::CUserCmd* cmd, Vector& aimPoint)
{
    if (IsTickValid(records[i].tick_count))
    {
        aimPoint = records[i].headPosition;
        cmd->tick_count = records[i].tick_count;
        return true;
    }
    return false;
}

llamaBT* llamaBacktrack = new llamaBT();