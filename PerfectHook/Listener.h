#pragma once
#include "singleton.hpp"
#include "MiscClasses.h"
#include "Interfaces.h"
class item_purchase
    : public singleton<item_purchase>
{
    class item_purchase_listener
        : public IGameEventListener2
    {
    public:
        void start()
        {
            g_EventManager->AddListener(this, "item_purchase", false);
        }
        void stop()
        {
            g_EventManager->RemoveListener(this);
        }
        void FireGameEvent(IGameEvent *event) override
        {
            singleton()->on_fire_event(event);
        }
        int GetEventDebugID(void) override
        {
            return 42 /*0x2A*/;
        }
    };
public:
    static item_purchase* singleton()
    {
        static item_purchase* instance = new item_purchase;
        return instance;
    }

    void initialize()
    {
        _listener.start();
    }

    void remove()
    {
        _listener.stop();
    }

    void on_fire_event(IGameEvent* event)
    {
        IClientEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
        if (!strcmp(event->GetName(), "item_purchase"))
        {
            auto _buyer = event->GetInt("userid");
            std::string _gun = event->GetString("weapon");

            if (strstr(_gun.c_str(), "molotov")
                || strstr(_gun.c_str(), "nade")
                || strstr(_gun.c_str(), "kevlar")
                || strstr(_gun.c_str(), "decoy")
                || strstr(_gun.c_str(), "suit")
                || strstr(_gun.c_str(), "flash")
                || strstr(_gun.c_str(), "vest")
                || strstr(_gun.c_str(), "cutter")
                || strstr(_gun.c_str(), "defuse")
                )  return;
            auto _player_index = g_Engine->GetPlayerForUserID(_buyer);
            IClientEntity* player = (IClientEntity*)g_EntityList->GetClientEntity(_player_index);
            player_info_t pinfo;
            
            if (player && local &&  player->GetTeamNum() != local->GetTeamNum() && g_Engine->GetPlayerInfo(_player_index, &pinfo))
            {
                if (g_ChatElement)
                {
                    _gun.erase(_gun.find("weapon_"), 7);
                    g_ChatElement->ChatPrintf(0, 0, " ""\x04""%s bought %s\n", pinfo.name, _gun.c_str());
                }
            }
        }
    }
private:
    item_purchase_listener  _listener;
};
