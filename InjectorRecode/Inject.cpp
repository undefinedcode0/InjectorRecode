#include "Inject.h"
#include "print_statements.h"

bool Inject::InjectDLL(const std::string& dllPath, const std::string& processName, int processId) {
    HANDLE hProcess = nullptr;
    if (processId != 0) {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    }
    else {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(entry);
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (Process32First(hSnap, &entry)) {
            do {
                std::wstring wProcessName(processName.begin(), processName.end());
                if (wProcessName == entry.szExeFile) {
                    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                    break;
                }
            } while (Process32Next(hSnap, &entry));
        }
        CloseHandle(hSnap);
    }

    if (!hProcess) {
        return false;
    }

    LPVOID pDllPath = VirtualAllocEx(hProcess, nullptr, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!pDllPath) {
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, pDllPath, dllPath.c_str(), dllPath.size() + 1, nullptr)) {
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    LPVOID pLoadLibrary = reinterpret_cast<LPVOID>(GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA"));
    if (!pLoadLibrary) {
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hRemoteThread = CreateRemoteThread(hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pLoadLibrary), pDllPath, 0, nullptr);
    if (!hRemoteThread) {
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hRemoteThread, INFINITE);

    CloseHandle(hRemoteThread);
    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return true;
}