

#pragma once

#include "RenderManager.h"



// Font Instances
namespace Render
{
	// Text functions
	namespace Fonts
	{
		DWORD Default;
		DWORD ESP;
		DWORD Guns;
		DWORD DroppedGuns;
		DWORD Defuse;
		DWORD Time;

	};
};

// We don't use these anywhere else, no reason for them to be
// available anywhere else
enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

// Initialises the rendering system, setting up fonts etc
void Render::Initialise()
{
	Fonts::Default = 0x1D; // MainMenu Font from vgui_spew_fonts 
	Fonts::ESP = I::Surface->FontCreate();
	Fonts::Guns = I::Surface->FontCreate();
	Fonts::Defuse = I::Surface->FontCreate();
	Fonts::Time = I::Surface->FontCreate();
	Fonts::DroppedGuns = I::Surface->FontCreate();




	I::Surface->SetFontGlyphSet(Fonts::ESP, "Tahoma", 11, 700, 0, 0, FONTFLAG_DROPSHADOW);
	I::Surface->SetFontGlyphSet(Fonts::Defuse, "Tahoma", 15, 700, 0, 0, FONTFLAG_DROPSHADOW);
	I::Surface->SetFontGlyphSet(Fonts::Guns, "Tahoma", 10, 700, 0, 0, FONTFLAG_DROPSHADOW);
	I::Surface->SetFontGlyphSet(Fonts::Time, "Arial", 13, 550, 0, 0, FONTFLAG_NONE);
	I::Surface->SetFontGlyphSet(Fonts::DroppedGuns, "Tahoma", 8, 700, 0, 0, FONTFLAG_DROPSHADOW);


}

RECT Render::GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	I::Engine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void Render::Clear(int x, int y, int w, int h, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Render::DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::Outline(int x, int y, int w, int h, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::DrawString2(DWORD font, int x, int y, Color color, DWORD alignment, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int width, height;
	I::Surface->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(r, g, b, a);
	I::Surface->DrawSetTextPos(x, y - height / 2);
	I::Surface->DrawPrintText(wbuf, wcslen(wbuf));
}


void Render::OutlineRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		Render::Outline(x + i, y, 1, height, colRainbow);
	}
}

void Render::Pixel(int x, int y, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawFilledRect(x, y, x + 1, y + 1);
}

void Render::Line(int x, int y, int x2, int y2, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawLine(x, y, x2, y2);
}

void Render::PolyLine(int *x, int *y, int count, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawPolyLine(x, y, count);
}

bool Render::WorldToScreen(Vector &in, Vector &out)
{
	const matrix3x4& worldToScreen = I::Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	out.z = 0; //Screen doesn't have a 3rd dimension.

	if (w > 0.001) //If the object is within view.
	{
		RECT ScreenSize = GetViewport();
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f); //Get the X dimension and push it in to the Vector.
		out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}

void Render::Text(int x, int y, Color color, DWORD font, const char* text, ...)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	I::Surface->DrawSetTextFont(font);

	I::Surface->DrawSetTextColor(color);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(wcstring, wcslen(wcstring));
	return;
}

void Render::Text(int x, int y, Color color, DWORD font, const wchar_t* text)
{
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(color);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(text, wcslen(text));
}

void Render::Text(int x, int y, DWORD font, const wchar_t* text)
{
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(text, wcslen(text));
}

void Render::Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	Text(x, y, color, font, logBuf);
}

RECT Render::GetTextSize(DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	I::Surface->GetTextSize(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}

void Render::GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < h; i++)
	{
		float fi = float(i), fh = float(h);
		float a = float(fi / fh);
		DWORD ia = DWORD(a * 255);
		Clear(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void Render::GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < w; i++)
	{
		float fi = float(i), fw = float(w);
		float a = float(fi / fw);
		DWORD ia = DWORD(a * 255);
		Clear(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void Render::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = I::Surface->CreateNewTextureID(true); //need to make a texture with procedural true
	unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

	I::Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	I::Surface->DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
	I::Surface->DrawSetTexture(Texture); // bind texture

	I::Surface->DrawTexturedPolygon(count, Vertexs);
}

void Render::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Render::Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = int(Vertexs[i].m_Position.x);
		y[i] = int(Vertexs[i].m_Position.y);
	}

	Render::PolyLine(x, y, count, colorLine);
}

void Render::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = int(Vertexs[i].m_Position.x);
		y[i] = int(Vertexs[i].m_Position.y);
	}

	Render::PolyLine(x, y, count, colorLine);
}

void Render::OutlineCircle(int x, int y, int r, int seg, Color color)
{
	I::Surface->DrawSetColor(0, 0, 0, 255);
	I::Surface->DrawOutlinedCircle(x, y, r - 1, seg);
	I::Surface->DrawOutlinedCircle(x, y, r + 1, seg);
	I::Surface->DrawSetColor(color);
	I::Surface->DrawOutlinedCircle(x, y, r, seg);
}