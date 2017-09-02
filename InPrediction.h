#pragma once
#include "HookIncludes.h"

typedef bool(__thiscall* InPrediction_)(PVOID);
InPrediction_ oInPrediction;
bool __stdcall hkInPrediction();


bool __stdcall hkInPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::Prediction.GetOriginalFunction(14);
	static IPrediction *ecxVal = I::Prediction;
	result = origFunc(ecxVal);
	if (menu.Visuals.NoVisualRecoil && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;
		float* m_LocalViewAngles = NULL;
		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}
		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();
		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}
	return result;
}