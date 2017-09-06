#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "DLLMain.h"
#include <time.h>
#include "BaseClient.h"
void IMisc::Init()
{

	// Any init
}
void IMisc::PaintTraverse()
{
}
void setclantag(const char* tag)
{
    static auto ClanTagOffset = U::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx");
    if (ClanTagOffset)
    {
        auto tag_ = std::string(tag);
        if (strlen(tag) > 0) {
            auto newline = tag_.find("\\n");
            auto tab = tag_.find("\\t");
            if (newline != std::string::npos) {
                tag_.replace(newline, newline + 2, "\n");
            }
            if (tab != std::string::npos) {
                tag_.replace(tab, tab + 2, "\t");
            }
        }
        static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
        dankesttSetClanTag(tag_.data(), tag_.data());
    }
}
const char* tuxlist[] =
{
    "AimTux owns me and all",
    "Your Windows p2c sucks my AimTux dry",
    "It's free as in FREEDOM!",
    "Tux only let me out so I could play this game, please be nice!",
    "Tux nutted but you keep sucken",
    ">tfw no vac on Linux",
};
const char* ezfragslist[] =
{
    "Think you could do better? Not without www.EZFrags.co.uk",
    "If I was cheating, I'd use www.EZFrags.co.uk",
    "I'm not using www.EZFrags.co.uk, you're just bad",
    "Visit www.EZFrags.co.uk for the finest public & private CS:GO cheats",
    "Stop being a noob! Get good with www.EZFrags.co.uk",
    "You just got pwned by www.EZFrags.co.uk, the #1 CS:GO cheat",
};
std::string animatedclantag;
Vector AutoStrafeView;
int iLastTime;
int topmemes123;
int xdmemes123;
char name[] = "xcheat.solutions";
bool bDone=false;
void IMisc::CreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket)
{

	IClientEntity *local = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
    I::Engine->GetViewAngles(AutoStrafeView);
    if (menu.Misc.Bhop && local->IsAlive())
    {
        if (pCmd->buttons & IN_JUMP && !(local->GetMoveType() & MOVETYPE_LADDER))
        {

            int iFlags = hack.pLocal()->GetFlags();
            if (!(iFlags & FL_ONGROUND))
                pCmd->buttons &= ~IN_JUMP;
        }
    }
    if(menu.Misc.syncclantag)
    {
        if (int(I::Globals->curtime) != iLastTime)
        {
            switch (int(I::Globals->curtime) % 20)
            {
            case 0: setclantag(XorStr("thook         p")); break;
            case 1: setclantag(XorStr("hook         pe")); break;
            case 2: setclantag(XorStr("ook         per")); break;
            case 3: setclantag(XorStr("ok         perf")); break;
            case 4: setclantag(XorStr("k         perfe")); break;
            case 5: setclantag(XorStr("         perfec")); break;
            case 6: setclantag(XorStr("        perfect")); break;
            case 7: setclantag(XorStr("       perfecth")); break;
            case 8: setclantag(XorStr("      perfectho")); break;
            case 9: setclantag(XorStr("     perfecthoo")); break;
           case 10: setclantag(XorStr("    perfecthook")); break;
           case 11: setclantag(XorStr("   perfecthook ")); break;
           case 12: setclantag(XorStr("  perfecthook  ")); break;
           case 13: setclantag(XorStr(" perfecthook   ")); break;
           case 14: setclantag(XorStr("perfecthook    ")); break;
           case 15: setclantag(XorStr("erfecthook     ")); break;
           case 16: setclantag(XorStr("rfecthook      ")); break;
           case 17: setclantag(XorStr("fecthook       ")); break;
           case 18: setclantag(XorStr("ecthook        ")); break;
           case 19: setclantag(XorStr("cthook         ")); break;
            }
        }

        iLastTime = int(I::Globals->curtime);
    }
    if (!menu.Misc.animatedclantag && animatedclantag.c_str() != G::AnimatedClantag)
    {
        animatedclantag = G::AnimatedClantag;
    }
    if (menu.Misc.animatedclantag && animatedclantag.length() > 1)
    {
        if (int(I::Globals->curtime) != iLastTime)
        {
            auto length = animatedclantag.length();
            animatedclantag.insert(0, 1, animatedclantag[length - 2]);
            animatedclantag.erase(length - 1, 1);

            setclantag(animatedclantag.c_str());
            iLastTime = int(I::Globals->curtime);
        }
    }
    static size_t lastTime = 0;
    if (GetTickCount() > lastTime)
    {

            xdmemes123 = xdmemes123 + 1;
            if (xdmemes123 >= 6)
                xdmemes123 = 0;
            switch(menu.Misc.spammer)
            {
            case 1:
                SayInChat(tuxlist[xdmemes123]);
                break;
            case 2:
                SayInChat(ezfragslist[xdmemes123]);
                break;
            }
            
            lastTime = GetTickCount() + 850;
      

    }
    if (menu.Misc.AutoStrafe)
        AutoStrafe(pCmd);

    if(menu.Misc.silentstealer)
    {
        static char namebuf[130]; 
        static ConVar* name = I::CVar->FindVar("name"); 
        if (name)
        {
            if (!bDone)
            {
                name->SetValue("\n­­­");
                bDone = true;
            }
            for (int i = 0; i < I::Engine->GetMaxClients(); i++)
            {
                auto entity = I::EntityList->GetClientEntity(i);
                if (!entity || local || entity == local || entity->GetTeamNum() == local->GetTeamNum())
                    continue;
                player_info_t info;
                if (!I::Engine->GetPlayerInfo(i, &info))
                    continue;
                if (strstr(info.name, "GOTV"))
                    continue;
                sprintf_s(namebuf, sizeof(namebuf) - 1, "%s ", info.name);
                name->SetValue(namebuf);
            }
        }
    }
    else if (!menu.Misc.silentstealer && bDone == true) bDone = false;
}

Vector GetAutostrafeView()
{
    return AutoStrafeView;
}

template<class T, class U>
inline T clamp(T in, U low, U high)
{
    if (in <= low)
        return low;
    else if (in >= high)
        return high;
    else
        return in;
}

inline float bitsToFloat(unsigned long i)
{
    return *reinterpret_cast<float*>(&i);
}


inline float FloatNegate(float f)
{
    return bitsToFloat(FloatBits(f) ^ 0x80000000);
}


void IMisc::AutoStrafe(CInput::CUserCmd *pCmd)
{
    IClientEntity* pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

    static float move = 450; //4?.f; // move = max(move, (abs(cmd->move.x) + abs(cmd->move.y)) * 0.5f);
    float s_move = move * 0.5065f;
    if (pLocal->GetMoveType() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER))
        return;
    if (pCmd->buttons & (IN_FORWARD | IN_MOVERIGHT | IN_MOVELEFT | IN_BACK))
        return;

    if (pCmd->buttons & IN_JUMP || !(pLocal->GetFlags() & FL_ONGROUND))
    {
        if (pLocal->GetVelocity().Length2D() >= menu.Misc.MinVel)
        {
            pCmd->forwardmove = move * 0.015f;
            pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

            if (pCmd->mousedx)
                pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

            static float strafe = pCmd->viewangles.y;

            float rt = pCmd->viewangles.y, rotation;
            rotation = strafe - rt;

            if (rotation < FloatNegate(I::Globals->interval_per_tick))
                pCmd->sidemove = -s_move;

            if (rotation > I::Globals->interval_per_tick)
                pCmd->sidemove = s_move;

            strafe = rt;
        }
    }
}