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
            I::EventManager->AddListener(this, "item_purchase", false);
        }
        void stop()
        {
            I::EventManager->RemoveListener(this);
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
        IClientEntity* local = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
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
            auto _player_index = I::Engine->GetPlayerForUserID(_buyer);
            IClientEntity* player = (IClientEntity*)I::EntityList->GetClientEntity(_player_index);
            player_info_t pinfo;
            
            if (player && local &&  player->GetTeamNum() != local->GetTeamNum() && I::Engine->GetPlayerInfo(_player_index, &pinfo))
            {
                if (I::ChatElement)
                {
                    _gun.erase(_gun.find("weapon_"), 7);
                    I::ChatElement->ChatPrintf(0, 0, " ""\x04""%s bought %s\n", pinfo.name, _gun.c_str());
                }
            }
        }
    }
private:
    item_purchase_listener  _listener;
};
