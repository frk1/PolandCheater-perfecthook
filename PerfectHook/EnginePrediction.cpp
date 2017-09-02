
#include "SDK.h"
#include "Hacks.h"
float m_flOldCurtime;
float m_flOldFrametime;
CMoveData m_MoveData;

int* m_pPredictionRandomSeed;

void StartPrediction(CInput::CUserCmd* pCmd) {
	IClientEntity *pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	static bool bInit = false;
	if (!bInit) {
		m_pPredictionRandomSeed = *(int**)(U::FindPattern("client.dll", (PBYTE)"\x8B\x0D\x00\x00\x00\x00\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04", "xx????x????x????xxx") + 2);
		bInit = true;
	}


	*m_pPredictionRandomSeed = pCmd->random_seed;


	m_flOldCurtime = I::Globals->curtime;
	m_flOldFrametime = I::Globals->frametime;

	I::Globals->curtime = pLocal->GetTickBase() * I::Globals->interval_per_tick;
	I::Globals->frametime = I::Globals->interval_per_tick;

	I::GameMovement->StartTrackPredictionErrors(pLocal);

	memset(&m_MoveData, 0, sizeof(m_MoveData));
	I::MoveHelper->SetHost(pLocal);
	I::Prediction->SetupMove(pLocal, pCmd, I::MoveHelper, &m_MoveData);
	I::GameMovement->ProcessMovement(pLocal, &m_MoveData);
	I::Prediction->FinishMove(pLocal, pCmd, &m_MoveData);

}

void EndPrediction(CInput::CUserCmd* pCmd) {
	IClientEntity *pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	I::GameMovement->FinishTrackPredictionErrors(pLocal);
	I::MoveHelper->SetHost(0);

	*m_pPredictionRandomSeed = -1;

	I::Globals->curtime = m_flOldCurtime;
	I::Globals->frametime = m_flOldFrametime;
}