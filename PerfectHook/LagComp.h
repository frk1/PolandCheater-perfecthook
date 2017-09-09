#pragma once
#include "SDK.h"

struct lbyRecords
{
    int tick_count;
    float lby;
    Vector headPosition;
};

class llamaBT
{
    lbyRecords records[64];
    int latest_tick;
    bool IsTickValid(int tick);
    void UpdateRecord(int i);
public:
    bool BacktrackNigger(int i, CInput::CUserCmd* cmd, Vector& aimPoint);
    void Update(int tick_count);
};

extern llamaBT* llamaBacktrack;


inline void legitLagComp(CInput::CUserCmd* cmd, IClientEntity* pLocal)
{
    if (menu.Legitbot.backtrack)
    {
        int bestTargetIndex = -1;
        float bestFov = FLT_MAX;
        player_info_t info;
        for (int i = 0; i < g_Engine->GetMaxClients(); i++)
        {
            auto entity = (IClientEntity*)g_EntityList->GetClientEntity(i);

            if (!entity || !pLocal)
                continue;

            if (entity == pLocal)
                continue;

            if (!g_Engine->GetPlayerInfo(i, &info))
                continue;

            if (entity->IsDormant())
                continue;


            if (entity->GetTeamNum() == pLocal->GetTeamNum())
                continue;


            if (entity->IsAlive())
            {

                float simtime = entity->GetSimulationTime();
                Vector hitboxPos = GetHitboxPosition(entity, 0);


                headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos, entity };
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
}