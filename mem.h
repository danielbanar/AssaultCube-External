#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
typedef uintptr_t Uintptr;
typedef uint64_t Uint64;
typedef uint32_t Uint32;
typedef uint16_t Uint16;
typedef uint8_t Uint8;
class Memory
{
public:
	HANDLE hProcess;
	DWORD dwPid;
	Memory(HANDLE handle, DWORD pid) : hProcess(handle), dwPid(pid) {};
	Memory(LPCSTR process_name)
	{
		dwPid = GetPID(process_name);
		hProcess = GetHandle(dwPid);
	}
	template<typename T> T read(Uintptr address)
	{
		T buffer;
		ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
		return buffer;
	}
	template<typename T> void write(Uintptr address, T value)
	{
		DWORD oldProtect;
		VirtualProtectEx(hProcess, (LPVOID)address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
		WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), nullptr);
		VirtualProtectEx(hProcess, (LPVOID)address, sizeof(T), oldProtect, &oldProtect);
	}
	Uintptr pPath(Uintptr baseAdress, std::vector<Uintptr> offsets)
	{
		Uintptr temp = read<Uintptr>(baseAdress);
		for (size_t i = 0; i < offsets.size(); i++)
		{
			temp = read<Uintptr>(temp + offsets[i]);
		}
		return temp;
	}
	Uintptr GetModuleBaseAddress(const char* moduleName)
	{
		DWORD dwBase = 0;
		do
		{
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwPid);
			if (hSnapshot == INVALID_HANDLE_VALUE)
				continue;
			MODULEENTRY32 ModuleEntry32;
			ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(hSnapshot, &ModuleEntry32)) {
				do
				{
					if (!strcmp(ModuleEntry32.szModule, (LPSTR)moduleName))
					{
						dwBase = (DWORD)ModuleEntry32.modBaseAddr;
						break;
					}
				} while (Module32Next(hSnapshot, &ModuleEntry32));
			}
			CloseHandle(hSnapshot);
		} while (!dwBase);
		return dwBase;
	}
	void patch(BYTE* dest, BYTE* src, size_t size)
	{
		DWORD oldProtect;
		VirtualProtectEx(hProcess, dest, size, PAGE_EXECUTE_READWRITE, &oldProtect);
		WriteProcessMemory(hProcess, dest, src, size, nullptr);
		VirtualProtectEx(hProcess, dest, size, oldProtect, &oldProtect);
	}
	void nop(BYTE* dest, size_t size)
	{
		BYTE* nopArray = new BYTE[size];
		memset(nopArray, 0x90, size);

		patch(dest, nopArray, size);
		delete[] nopArray;
	}
private:
	DWORD GetPID(LPCSTR process_name)
	{
		DWORD dwPid;
		HWND hwnd = FindWindow(0, process_name);
		GetWindowThreadProcessId(hwnd, &dwPid);
		return dwPid;
	}
	HANDLE GetHandle(DWORD dwPid)
	{
		return OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
	}
};