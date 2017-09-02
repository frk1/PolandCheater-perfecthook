#pragma once

#include "MiscDefinitions.h"

#include "offsets.h"
#include "Vector.h"
#include "MiscClasses.h"
#include "Vector2D.h"

typedef unsigned short MaterialHandle_t;

class IMaterial
{
public:
	const char* GetName()
	{
		typedef const char* (__thiscall* GetNameFn)(void*);
		return call_vfunc<GetNameFn>(this, 0)(this);
	}

	const char* GetTextureGroupName()
	{
		typedef const char* (__thiscall* GetTextureGroupNameFn)(void*);
		return call_vfunc<GetTextureGroupNameFn>(this, 1)(this);
	}

	void IncrementReferenceCount()
	{
		typedef void(__thiscall* IncrementReferenceCountFn)(void*);
		call_vfunc<IncrementReferenceCountFn>(this, 14)(this);
	}

	void AlphaModulate(float alpha)
	{
		typedef void(__thiscall* AlphaModulateFn)(void*, float);
		call_vfunc<AlphaModulateFn>(this, 27)(this, alpha);
	}

	void ColorModulate(float r, float g, float b)
	{
		typedef void(__thiscall* ColorModulateFn)(void*, float, float, float);
		call_vfunc<ColorModulateFn>(this, 28)(this, r, g, b);
	}

	void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on)
	{
		typedef void(__thiscall* SetMaterialVarFlagFn)(void*, MaterialVarFlags_t, bool);
		call_vfunc<SetMaterialVarFlagFn>(this, 29)(this, flag, on);
	}
};
#define VirtualFn( cast )typedef cast( __thiscall* OriginalFn )
class CMaterialSystem
{
public:
	IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues)
	{
		VirtualFn(IMaterial*)(PVOID, const char*, KeyValues*);
		return call_vfunc< OriginalFn >(this, 83)(this, pMaterialName, pVMTKeyValues);
	}
	IMaterial* FindMaterial(const char* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = 0)
	{
		VirtualFn(IMaterial*)(PVOID, const char*, const char*, bool, const char*);
		return call_vfunc< OriginalFn >(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
	MaterialHandle_t FirstMaterial()
	{
		VirtualFn(MaterialHandle_t)(PVOID);
		return call_vfunc< OriginalFn >(this, 86)(this);
	}
	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		VirtualFn(MaterialHandle_t)(PVOID, MaterialHandle_t);
		return call_vfunc< OriginalFn >(this, 87)(this, h);
	}
	MaterialHandle_t InvalidMaterial()
	{
		VirtualFn(MaterialHandle_t)(PVOID);
		return call_vfunc< OriginalFn >(this, 88)(this);
	}
	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		VirtualFn(IMaterial*)(PVOID, MaterialHandle_t);
		return call_vfunc< OriginalFn >(this, 89)(this, h);
	}
	int	GetNumMaterials()
	{
		VirtualFn(int)(PVOID);
		return call_vfunc< OriginalFn >(this, 90)(this);
	}

};

class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial* material, OverrideType_t type = OVERRIDE_NORMAL, int idk = NULL)
	{
		typedef void (__thiscall* Fn)(void*, IMaterial*, OverrideType_t, int);
		return call_vfunc<Fn>(this, 1)(this, material, type, idk);
	}
};




class CModelInfo
{
public:
	inline void* GetModel(int Index) {
		return call_vfunc<void*(__thiscall *)(void*, int)>(this, 1)(this, Index);
	}

	inline int GetModelIndex(const char* Filename) {
		return call_vfunc<int(__thiscall *)(void*, const char*)>(this, 2)(this, Filename);
	}

	inline const char* GetModelName(const void* Model) {
		return call_vfunc<const char*(__thiscall *)(void*, const void*)>(this, 3)(this, Model);
	}

	void GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial)
	{
		typedef void *(__thiscall* oGetModelMaterials)(PVOID, const model_t*, int, IMaterial**);
		call_vfunc<oGetModelMaterials>(this, 17)(this, model, count, ppMaterial);
	}

	studiohdr_t* GetStudiomodel(const model_t* mod)
	{
		typedef studiohdr_t*(__stdcall* oGetStudiomodel)(const model_t*);
		return call_vfunc<oGetStudiomodel>(this, 30)(mod);
	}
};

class CVRenderView
{
public:
	void SetBlend(float alpha)
	{
		typedef void (__thiscall* oDrawModelExecute)(PVOID, float);
		return call_vfunc<oDrawModelExecute>(this, 4)(this, alpha);
	}

	void SetColorModulation(float const* colors)
	{
		typedef void (__thiscall* oDrawModelExecute)(PVOID, float const*);
		return call_vfunc<oDrawModelExecute>(this, 6)(this, colors);
	}
};
struct vrect_t;
enum DrawFlags_t
{
	DF_RENDER_REFRACTION = 0x1,
	DF_RENDER_REFLECTION = 0x2,
	DF_CLIP_Z = 0x4,
	DF_CLIP_BELOW = 0x8,
	DF_RENDER_UNDERWATER = 0x10,
	DF_RENDER_ABOVEWATER = 0x20,
	DF_RENDER_WATER = 0x40,
	DF_UNUSED1 = 0x100,
	DF_WATERHEIGHT = 0x200,
	DF_UNUSED2 = 0x400,
	DF_DRAWSKYBOX = 0x800,
	DF_FUDGE_UP = 0x1000,
	DF_DRAW_ENTITITES = 0x2000,
	DF_UNUSED3 = 0x4000,
	DF_UNUSED4 = 0x8000,
	DF_UNUSED5 = 0x10000,
	DF_SAVEGAMESCREENSHOT = 0x20000,
	DF_CLIP_SKYBOX = 0x40000,
	DF_SHADOW_DEPTH_MAP = 0x100000    // Currently rendering a shadow depth map
};
class IViewRender
{
public:
	virtual void                Init(void) = 0;
	virtual void                LevelInit(void) = 0;
	virtual void                LevelShutdown(void) = 0;
	virtual void                Shutdown(void) = 0;
	virtual void                OnRenderStart() = 0;
	virtual    void                Render(vrect_t *rect) = 0;
	virtual void                RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw) = 0;
	virtual int                 GetDrawFlags() = 0;
	virtual void                StartPitchDrift(void) = 0;
	virtual void                StopPitchDrift(void) = 0;
	virtual void*               GetFrustum() = 0;
	virtual bool                ShouldDrawBrushModels(void) = 0;
	virtual const CViewSetup*   GetPlayerViewSetup(void) const = 0;
	virtual const CViewSetup*   GetViewSetup(void) const = 0;
	virtual void                DisableVis(void) = 0;
	virtual int                 BuildWorldListsNumber() const = 0;
	virtual void                SetCheapWaterStartDistance(float flCheapWaterStartDistance) = 0;
	virtual void                SetCheapWaterEndDistance(float flCheapWaterEndDistance) = 0;
	virtual void                GetWaterLODParams(float &flCheapWaterStartDistance, float &flCheapWaterEndDistance) = 0;
	virtual void                DriftPitch(void) = 0;
	virtual void                SetScreenOverlayMaterial(IMaterial *pMaterial) = 0;
	virtual IMaterial*          GetScreenOverlayMaterial() = 0;
	virtual void                WriteSaveGameScreenshot(const char *pFilename) = 0;
	virtual void                WriteSaveGameScreenshotOfSize(const char *pFilename, int width, int height) = 0;
	virtual void                QueueOverlayRenderView(const CViewSetup &view, int nClearFlags, int whatToDraw) = 0;
	virtual float               GetZNear() = 0;
	virtual float               GetZFar() = 0;
	virtual void                GetScreenFadeDistances(float *min, float *max) = 0;
	virtual IClientEntity*       GetCurrentlyDrawingEntity() = 0;
	virtual void                SetCurrentlyDrawingEntity(IClientEntity *pEnt) = 0;
};