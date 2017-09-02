
#pragma once

#include "SDK.h"

namespace MiscFunctions
{
	void NormaliseViewAngle(Vector &angle);
	bool IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID);
	bool IsKnife(void* weapon);
	bool IsPistol(void* weapon);
	bool IsSniper(void* weapon);
	bool IsGrenade(void* weapon);
	//	int GetPlayerCompRank(IClientEntity* pEntity);
};

// Trace Line Memes
void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
	const IClientEntity *ignore, int collisionGroup, trace_t *ptr);

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);

bool IsBreakableEntity(IClientEntity* ent);

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace);

void SayInChat(const char *text);
void SetName(const char *text);
Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox);

void ForceUpdate();