#pragma once
#include "draw.h"
#pragma warning(disable:4244)
Drawing::Drawing(HWND hwnd) : hwndGDI(hwnd)
{
	Gdiplus::GdiplusStartupInput startupInput;
	Gdiplus::GdiplusStartup(&m_token, &startupInput, NULL);
}

void Drawing::Rect(const Coord& feetPos, const Coord& headPos)
{
	int height = feetPos.y - headPos.y, width = (float)height * 0.5555f;
	g.DrawRectangle(&res.redPen, headPos.x - width * 0.5, headPos.y, width, height);
}
void Drawing::String(const Coord& pos, const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wc = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, wc, size, text, size - 1);
	g.DrawString(wc, size, &res.font, PointF(pos.x, pos.y), &res.whiteBrush);
}
void Drawing::Line(const Coord& forwardPos, const Coord& headPos)
{
	g.DrawLine(&res.whitePen, headPos.x, headPos.y, forwardPos.x, forwardPos.y);
}
void Drawing::Health(const Coord& feetPos, const Coord& headPos, int health)
{
	Pen pen(Color((100 - health)*2.55f, health*2.55f, 0), 2);
	int height = feetPos.y - headPos.y, width = (float)height * 0.5555f;
	int hph = (feetPos.y - headPos.y)*0.01f*health;
	g.DrawLine(&pen, headPos.x - width * 0.5 - 4, headPos.y, headPos.x - width * 0.5 - 4, headPos.y + hph);
}
void Drawing::ESP(const Coord& feetPos, const Coord& headPos, const Coord& forward, const PlayerEntity& entity)
{
	int width = (float)(feetPos.y - headPos.y) * 0.2777f;
	Coord namePos(headPos.x - width - 3, headPos.y - 20);
	String(namePos, entity.name);
	Health(feetPos, headPos, entity.health);
	Line(forward, headPos);
	Rect(feetPos, headPos);
}
void Drawing::Clear()
{
	g.Clear(trColor);
}