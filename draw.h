#pragma once
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <windows.h>
#include <wingdi.h>
#include "structs.h"
#include <gdiplus.h>
#include <gdiplusheaders.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

struct GDIResources
{
	Font font;
	SolidBrush whiteBrush;
	Pen whitePen;
	Pen redPen;
};

class GdiPlusInit
{
public:
	GdiPlusInit()
	{
		Gdiplus::GdiplusStartupInput startupInput;
		Gdiplus::GdiplusStartup(&m_token, &startupInput, NULL);
	}

	~GdiPlusInit()
	{
		if (m_token)
			Gdiplus::GdiplusShutdown(m_token);
	}

	GdiPlusInit(const GdiPlusInit&) = delete;
	GdiPlusInit& operator=(const GdiPlusInit&) = delete;

private:
	ULONG_PTR m_token = 0;
};



class Drawing : GdiPlusInit
{
	FontFamily gdiFontFamily{ L"Tahoma" };
public:
	Drawing(HWND hwnd);
	void String(const Coord&, const char*);
	void Health(const Coord&, const Coord&, int);
	void Line(const Coord&, const Coord&);
	void Rect(const Coord&, const Coord&);
	void ESP(const Coord& feetPos, const Coord& headPos, const Coord& forward, const PlayerEntity& entity);
	void Clear();
	GDIResources res{ { &gdiFontFamily, 13, FontStyleRegular, UnitPixel }, { Color(255, 255, 255) }, { Color(255, 255, 255), 1 }, { Color(255, 0, 0), 2 } };
private:
	HWND hwndGDI;
	Graphics g{ GetDC(hwndGDI) };
	Color trColor{ 69,69,0 };
	ULONG_PTR m_token = 0;
};