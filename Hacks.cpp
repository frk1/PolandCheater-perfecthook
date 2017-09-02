
#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "ESP.h"

#include "RageBot.h"
#include "MiscHacks.h"
#include "LegitBot.h"
IESP Esp;
IMisc Misc;
IRage Rage;
ILegit Legit;
void Hacks::SetupHacks()
{

	Esp.Init();
	Misc.Init();
	Rage.Init();
	Legit.Init();

	hack.RegisterHack(&Esp);
	hack.RegisterHack(&Misc);
	hack.RegisterHack(&Rage);
	hack.RegisterHack(&Legit);

	hack.Ready();
}

void SpecList()
{
	IClientEntity *pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	RECT scrn = Render::GetViewport();
	int kapi = 0;

	if (pLocal)
	{
		for (int i = 0; i < I::EntityList->GetHighestEntityIndex(); i++)
		{
			// Get the entity
			IClientEntity *pEntity = I::EntityList->GetClientEntity(i);
			player_info_t pinfo;
			if (pEntity && pEntity != pLocal)
			{
				if (I::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
				{
					HANDLE obs = pEntity->GetObserverTargetHandle();
					if (obs)
					{
						IClientEntity *pTarget = I::EntityList->GetClientEntityFromHandle(obs);
						player_info_t pinfo2;
						if (pTarget && pTarget->GetIndex() == pLocal->GetIndex())
						{
							if (I::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
							{
									char buf[255]; sprintf_s(buf, "%s", pinfo.name);
									RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, buf);
									Render::Clear(scrn.right - 100, (scrn.top + 1) + (16 * kapi), 101, 16, Color(0, 0, 0, 140));
									Render::Text(scrn.right - TextSize.right - 4, (scrn.top) + (16 * kapi), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 255), Render::Fonts::ESP, buf);
									kapi++;
							}
						}
					}
				}
			}
		}
	}


	Render::Outline(scrn.right - 100, (scrn.top) - 1, 101, (16 * kapi) + 2, Color(23, 23, 23, 255));
	Render::Outline(scrn.right - 100, (scrn.top), 100, (16 * kapi), Color(90, 90, 90, 255));
}

void DLight()
{
	IClientEntity *pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	RECT scrn = Render::GetViewport();

	// Loop through all active entitys
	for (int i = 0; i < I::EntityList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = I::EntityList->GetClientEntity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pLocal != nullptr && pEntity != nullptr && pEntity != pLocal)
		{
			if (I::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive() && !pEntity->IsDormant())
			{
				if (pLocal->GetTeamNum() != pEntity->GetTeamNum())
				{
					dlight_t* pElight = I::Dlight->CL_AllocElight(i);
					pElight->origin = pEntity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
					pElight->radius = 300.0f;
					pElight->color.b = 36;
					pElight->color.g = 224;
					pElight->color.r = 41;
					pElight->die = I::Globals->curtime + 0.05f;
					pElight->decay = pElight->radius / 5.0f;
					pElight->key = i;

					dlight_t* pDlight = I::Dlight->CL_AllocDlight(i);
					pDlight->origin = pEntity->GetOrigin();
					pDlight->radius = 300.0f;
					pDlight->color.b = 36;
					pDlight->color.g = 224;
					pDlight->color.r = 41;
					pDlight->die = I::Globals->curtime + 0.05f;
					pDlight->decay = pDlight->radius; // / 5.0f;
					pDlight->key = i;
				}
			}
		}
	}
}

void Hacks::DrawHacks()
{
	if (menu.Misc.SpecList)
		if(hack.pLocal() && hack.pLocal()->IsAlive())
			SpecList();
	hack.PaintTraverse();

	if (!menu.Visuals.Enabled)
		return;





	if (menu.Visuals.DLight)
		DLight();



}

void Hacks::MoveHacks(CInput::CUserCmd *pCmd, bool& bSendPacket)
{
	int AirStuckKey = menu.Misc.AirStuckKey;
	if (AirStuckKey != 0 && G::PressedKeys[AirStuckKey])
	{
		pCmd->tick_count = INT_MAX;
	}

	Vector origView = pCmd->viewangles;
	hack.CreateMove(pCmd, bSendPacket);


}


//---------------------------------------------------------------------//
HackManager hack;

void HackManager::RegisterHack(CHack* hake)
{
	Hacks.push_back(hake);
	hake->Init();
}

void HackManager::PaintTraverse()
{
	if (!IsReady)
		return;

	pLocalInstance = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->PaintTraverse();
	}
}

// Handle all the move hakes
void HackManager::CreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket)
{
	if (!IsReady)
		return;


	pLocalInstance = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->CreateMove(pCmd, bSendPacket);
	}
}

IClientEntity* HackManager::pLocal()
{
	return pLocalInstance;
}


void HackManager::Ready()
{
	IsReady = true;
}