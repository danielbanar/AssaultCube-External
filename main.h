#pragma once
#include <directxmath.h>
#include <iostream>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <windows.h>
#include <wingdi.h>
#include <iostream>
#include <string>
#include "mem.h"
#include "structs.h"
#include "math.h"
#include "draw.h"
#define OFFSET(type,base)	(Uintptr)base + (Uintptr)&((type)0)

HWND CreateOverlay(HWND hwndGameWindow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void Noclip();
