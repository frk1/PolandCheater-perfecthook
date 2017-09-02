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
