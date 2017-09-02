#pragma once
#include "HookIncludes.h"
#include "Renderer.h"
#include "Interface.h"
#include <d3d9.h>
IDirect3DDevice9* d3d9_device;
typedef HRESULT(__stdcall *EndScene_t) (IDirect3DDevice9*);

typedef HRESULT(__stdcall *Reset_t) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

EndScene_t	g_fnOriginalEndScene = nullptr;
Reset_t		g_fnOriginalReset = nullptr;

HRESULT __stdcall hkReset(IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params) {

	if (!renderer->IsReady())
		return g_fnOriginalReset(thisptr, params);


	ImGui_ImplDX9_InvalidateDeviceObjects();
	renderer->~Renderer();

	HRESULT result = g_fnOriginalReset(thisptr, params);

	ImGui_ImplDX9_CreateDeviceObjects();

	renderer->Initialize(FindWindowA("Valve001", NULL), thisptr);

	return result;
}

HRESULT __stdcall hkEndScene(IDirect3DDevice9* thisptr) {



	static bool mouse_enabled = false;
	thisptr->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	bool is_renderer_active = renderer->IsActive();

	if (is_renderer_active) {
		if (mouse_enabled) {
			I::Engine->ClientCmd_Unrestricted("cl_mouseenable 0");
			mouse_enabled = false;
		}
	}
	else {
		if (!mouse_enabled) {
			I::Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
			mouse_enabled = true;
		}
	}

	ImGui::GetIO().MouseDrawCursor = is_renderer_active;

	if (!is_renderer_active)
		return g_fnOriginalEndScene(thisptr);

	ImGui_ImplDX9_NewFrame();

	RenderInterface();

	ImGui::Render();

	return g_fnOriginalEndScene(thisptr);
}

typedef HRESULT(_stdcall *Present_T)(void*, const RECT*, RECT*, HWND, RGNDATA*);
Present_T Present_O;
HRESULT _stdcall Present_H(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
    if (!renderer->IsReady())
    {
        renderer->Initialize(FindWindowA("Valve001", NULL), pDevice);
    }
    static bool mouse_enabled = false;
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
    bool is_renderer_active = renderer->IsActive();

    if (is_renderer_active) {
        if (mouse_enabled) {
            I::Engine->ClientCmd_Unrestricted("cl_mouseenable 0");
            mouse_enabled = false;
        }
    }
    else {
        if (!mouse_enabled) {
            I::Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
            mouse_enabled = true;
        }
    }

    ImGui::GetIO().MouseDrawCursor = is_renderer_active;

    if (!is_renderer_active)
        return Present_O(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

    ImGui_ImplDX9_NewFrame();

    RenderInterface();

    ImGui::Render();

    return Present_O(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
