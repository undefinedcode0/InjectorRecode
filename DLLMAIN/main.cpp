#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
		MessageBox(NULL, L"DLL_PROCESS_ATTACH", L"DLL_PROCESS_ATTACH", MB_OK | MB_TOPMOST);
        break;
    }
    return TRUE;
}
