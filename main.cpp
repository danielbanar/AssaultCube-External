#pragma once
#include "draw.h"
#include "main.h"

HWND hwndGame = FindWindow(0, "AssaultCube");
HWND hwndGDI = CreateOverlay(hwndGame);

MSG messages;
Memory mem("AssaultCube");
Uintptr hModule = mem.GetModuleBaseAddress("ac_client.exe");

Uintptr pEntityList = mem.read<Uintptr>(hModule + 0x10F4F8);//0x110D90
PlayerEntity* pLocalPlayer = mem.read<PlayerEntity*>(hModule + 0x109B74);
PlayerEntity localPlayer = mem.read<PlayerEntity>((Uintptr)pLocalPlayer);

::Matrix viewMatrix;
bool fullscreen = mem.read<bool>(hModule + 0x110CB0);
Coord screenResolution = mem.read<Coord>(hModule + 0x110C94);

Vec3 np;
bool bNoclip = false;
Uint32 iFrames = 0;

int main()
{
	Drawing draw(hwndGDI);
	//Custom colors, font...
	//FontFamily gdiFontFamily{ L"Consolas" };
	//GDIResources lmao{ { &gdiFontFamily, 13, FontStyleRegular, UnitPixel }, { Color(255, 255, 255) }, { Color(255, 255, 255), 1 }, { Color(255, 0, 0), 2 } };
	//memcpy(&draw.res, &lmao, sizeof(lmao));
	std::vector<PlayerEntity> entityList;

	RECT rectGDI;
	std::cout << "Process ID: " << mem.dwPid << std::endl;
	std::cout << "Process handle: " << (HANDLE*)mem.hProcess << std::endl;
	std::cout << "Game HWND: " << (HWND*)hwndGame << std::endl;
	std::cout << "GDI HWND: " << (HWND*)hwndGDI << std::endl;
	std::cout << "Module handle: " << (Uintptr*)hModule << std::endl;
	std::cout << "pLocalPlayer: " << (PlayerEntity*)pLocalPlayer << std::endl;
	std::cout << "pEntityList: " << (Uintptr*)pEntityList << std::endl;
	std::cout << (fullscreen ? "fullscreen " : "windowed ") << screenResolution.x << "x" << screenResolution.y << std::endl;
	std::cout << "_________________________" << std::endl;

	//Actual cheat
	while (true)
	{
		draw.Clear();

		if (!FindWindow(0, "AssaultCube"))
		{
			return 0;
		}
		else if (GetAsyncKeyState(VK_END) & 0x1)
		{
			bNoclip = false;
			Noclip();
			return 0;
		}

		fullscreen = mem.read<bool>(hModule + 0x110CB0);
		screenResolution = mem.read<Coord>(hModule + 0x110C94);
		pEntityList = mem.read<Uintptr>(hModule + 0x10F4F8);
		pLocalPlayer = mem.read<PlayerEntity*>(hModule + 0x109B74);
		localPlayer = mem.read<PlayerEntity>((Uintptr)pLocalPlayer);
		size_t entityCount = mem.read<size_t>(hModule + 0x10F500);
		viewMatrix = mem.read<::Matrix>(hModule + 0x101AE8);
		if (entityList.size() != entityCount)
			entityList.resize(entityCount);
		if (entityCount > 1)
		{
			for (size_t i = 0; i < entityCount; i++)
			{
				if (i == 0)
				{
					entityList[i] = localPlayer;
				}
				else
				{
					entityList[i] = mem.read<PlayerEntity>(mem.read<Uintptr>(pEntityList + i * 4));//i-1
				}
			}
		}
		if (entityCount > 1)
		{
			//AIMBOT
			int bestEntity = 0; // LocalPlayer
			float bestDistance = 9999.f;
			if (GetAsyncKeyState(VK_CAPITAL) & 0x8000)
			{
				for (size_t i = 1; i < entityCount; i++)
				{
					if (AngleDistance(localPlayer.viewAngles, CalcAngle(entityList[i].eyePos, localPlayer.eyePos)) < bestDistance && entityList[i].health > 0)
					{
						bestDistance = AngleDistance(localPlayer.viewAngles, CalcAngle(entityList[i].eyePos, localPlayer.eyePos));
						bestEntity = i;
					}
				}
				if (bestEntity)
				{
					Angle target = CalcAngle(entityList[bestEntity].eyePos, localPlayer.eyePos);
					mem.write<Angle>(OFFSET(PlayerEntity*, pLocalPlayer)->viewAngles, target);
					mem.write<Angle>(OFFSET(PlayerEntity*, pLocalPlayer)->viewAngles, target);
				}
			}

			//ESP
			for (size_t i = 1; i < entityCount; i++)
			{
				Coord feetScreen = WorldToScreen(entityList[i].pos, viewMatrix, screenResolution.x, screenResolution.y);
				Coord headScreen = WorldToScreen(entityList[i].eyePos, viewMatrix, screenResolution.x, screenResolution.y);
				Coord forward = WorldToScreen(entityList[i].eyePos + Forward(entityList[i].viewAngles)*3.f, viewMatrix, screenResolution.x, screenResolution.y);
				//Draw
				if (entityList[i].Alive())
				{
					draw.ESP(feetScreen, headScreen, forward, entityList[i]);
				}
			}
		}
		if (GetAsyncKeyState('G') & 0x1)
			bNoclip = !bNoclip;
		Noclip();
		mem.write<int>(OFFSET(PlayerEntity*, pLocalPlayer)->health, 999);
		mem.write<int>(mem.pPath((Uintptr)pLocalPlayer + 0x378, { 0x14 }), 999);
		if (iFrames % 10 == 0)
		{
			GetWindowRect(hwndGame, &rectGDI);
			rectGDI.top += 25;
			MoveWindow(hwndGDI, rectGDI.left, rectGDI.top, rectGDI.right - rectGDI.left, rectGDI.bottom - rectGDI.top, false);
			//Hide window when alt tabing
			if (GetForegroundWindow() != hwndGame)
			{
				ShowWindow(hwndGDI, SW_HIDE);
			}
			else
			{
				ShowWindow(hwndGDI, SW_SHOW);
				SetForegroundWindow(hwndGame);
			}
		}
		if (PeekMessage(&messages, hwndGDI, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
		iFrames++;
		Sleep(10);
	}
}

HWND CreateOverlay(HWND hwndGameWindow)
{
	//Garbage
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = 0;
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = CreateSolidBrush(RGB(69, 69, 0));//Invisible color
	windowClass.lpszClassName = "lmao";
	RegisterClassEx(&windowClass);
	//Getting games window rect
	RECT wRect;
	GetWindowRect(hwndGameWindow, &wRect);
	//Creating window with game window's rect
	HWND hwnd = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED, "lmao", "", WS_POPUP, wRect.left, wRect.top, wRect.right - wRect.left, wRect.bottom - wRect.top, 0, 0, 0, 0);
	ShowWindow(hwnd, SW_SHOW);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLongA(hwnd, GWL_EXSTYLE));
	SetLayeredWindowAttributes(hwnd, RGB(69, 69, 0), 0, LWA_COLORKEY);
	return hwnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CHAR:
		if (wparam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}
void Noclip()
{
	if (!bNoclip)
	{
		np = localPlayer.pos;
		mem.patch((BYTE*)hModule + 0x5B285, (BYTE*)"\x01\x4B\x54", 3);
	}
	else
	{
		if (GetAsyncKeyState('W') & 0x8000)
			np += Forward(localPlayer.viewAngles);
		if (GetAsyncKeyState('S') & 0x8000)
			np -= Forward(localPlayer.viewAngles);
		if (GetAsyncKeyState('A') & 0x8000)
			np -= Right(localPlayer.viewAngles);
		if (GetAsyncKeyState('D') & 0x8000)
			np += Right(localPlayer.viewAngles);
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			np += Up();
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
			np -= Up();

		mem.write<Vec3>(OFFSET(PlayerEntity*, pLocalPlayer)->pos, np);
		mem.nop((BYTE*)hModule + 0x5B285, 3);
		mem.write<int>(OFFSET(PlayerEntity*, pLocalPlayer)->airtime, 0);
	}
}
