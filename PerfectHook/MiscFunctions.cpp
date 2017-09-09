

#include "MiscFunctions.h"
#include "Utilities.h"
#include "Autowall.h"
#include "RenderManager.h"

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
	const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)U::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF0\x83\xEC\x7C\x56\x52", "xxxxxxxxxxx");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	static DWORD dwAddress = U::FindPattern("client.dll", (BYTE*)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx");

	if (!dwAddress)
		return;

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}

bool IsBreakableEntity(IClientEntity* ent)
{
	typedef bool(__thiscall* IsBreakbaleEntity_t)(IClientEntity*);
	IsBreakbaleEntity_t IsBreakbaleEntityFn = (IsBreakbaleEntity_t)U::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68", "xxxxxxxxxxx");
	if (IsBreakbaleEntityFn)
		return IsBreakbaleEntityFn(ent);
	else
		return false;
}

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)U::FindPattern("client.dll", (BYTE*)"\x55\x8B\xEC\x83\xEC\x30\xF3\x0F\x10\x75", "xxxxxxxxxx");

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

void MiscFunctions::NormaliseViewAngle(Vector &angle)
{
	while (angle.y <= -180) angle.y += 360;
	while (angle.y > 180) angle.y -= 360;
	while (angle.x <= -180) angle.x += 360;
	while (angle.x > 180) angle.x -= 360;


	if (angle.x > 89.0f)
		angle.x = 89.0f;
	else if (angle.x < -89.0f)
		angle.x = -89.0f;

	if (angle.y > 179.99999f)
		angle.y = 179.99999f;
	else if (angle.y < -179.99999f)
		angle.y = -179.99999f;

	angle.z = 0;
}


char shit[16];
trace_t Trace;
char shit2[16];
IClientEntity* entCopy;

bool MiscFunctions::IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID)
{
	if (BoneID < 0) return false;

	entCopy = pEntity;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = GetHitboxPosition(pEntity, BoneID);//pEntity->GetBonePos(BoneID); //pvs fix disabled




	//g_Trace->TraceRay(Ray,MASK_SOLID, NULL/*&filter*/, &Trace);
	UTIL_TraceLine(start, end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt == entCopy)
	{
		return true;
	}

	if (Trace.fraction == 1.0f)
	{
		return true;
	}

	return false;

}

bool MiscFunctions::IsKnife(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CKnife || pWeaponClass->m_ClassID == (int)ClassID::CC4 || pWeaponClass->m_ClassID == (int)ClassID::CKnifeGG)
		return true;
	else
		return false;
}

bool MiscFunctions::IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CDEagle || pWeaponClass->m_ClassID == (int)ClassID::CWeaponElite || pWeaponClass->m_ClassID == (int)ClassID::CWeaponFiveSeven || pWeaponClass->m_ClassID == (int)ClassID::CWeaponGlock || pWeaponClass->m_ClassID == (int)ClassID::CWeaponHKP2000 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponP250 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponP228 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponTec9 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponUSP)
		return true;
	else
		return false;
}

bool MiscFunctions::IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CWeaponAWP || pWeaponClass->m_ClassID == (int)ClassID::CWeaponSSG08 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponSCAR20 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponG3SG1)
		return true;
	else
		return false;
}

bool MiscFunctions::IsGrenade(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CDecoyGrenade || pWeaponClass->m_ClassID == (int)ClassID::CHEGrenade || pWeaponClass->m_ClassID == (int)ClassID::CIncendiaryGrenade || pWeaponClass->m_ClassID == (int)ClassID::CMolotovGrenade || pWeaponClass->m_ClassID == (int)ClassID::CSensorGrenade || pWeaponClass->m_ClassID == (int)ClassID::CSmokeGrenade || pWeaponClass->m_ClassID == (int)ClassID::CFlashbang)
		return true;
	else
		return false;
}


void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	g_Engine->ClientCmd_Unrestricted(buffer);
}

float GenerateRandomFloat(float Min, float Max)
{
	float randomized = (float)rand() / (float)RAND_MAX;
	return Min + randomized * (Max - Min);
}





Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
    matrix3x4 matrix[128];


    if (!pEntity->SetupBones(matrix, 128, 0x00000100, pEntity->GetSimulationTime()))
        return Vector(0, 0, 0);



    studiohdr_t* hdr = g_ModelInfo->GetStudiomodel(pEntity->GetModel());
    mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

    mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

    if (!hitbox)
        return Vector(0, 0, 0);

    Vector vMin, vMax, vCenter, sCenter;
    VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
    VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
    vCenter = (vMin + vMax) *0.5f;
    return vCenter;
}

Vector GetHitboxPositionFromMatrix(IClientEntity* pEntity, matrix3x4 matrix[128], int Hitbox)
{

    studiohdr_t* hdr = g_ModelInfo->GetStudiomodel(pEntity->GetModel());
    mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

    mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

    if (!hitbox)
        return Vector(0, 0, 0);

    Vector vMin, vMax, vCenter, sCenter;
    VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
    VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
    vCenter = (vMin + vMax) *0.5f;
    return vCenter;
}
