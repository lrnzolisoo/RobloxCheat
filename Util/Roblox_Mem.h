#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <algorithm>

struct Pattern {
    std::vector<BYTE> bytes;
    std::string mask;
};

class Roblox_Mem {
public:
    static inline DWORD processId = 0;
    static inline HANDLE handle = nullptr;
    static inline std::mutex resultsMutex;
    static inline std::vector<void*> foundAddresses;

    static HANDLE GetProcessByName(const char* name) {
        processId = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 process;
        process.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(snapshot, &process)) {
            do {
                if (strcmp(process.szExeFile, name) == 0) {
                    processId = process.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &process));
        }
        CloseHandle(snapshot);

        if (processId != 0) {
            HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
            handle = hProcess;
            return hProcess;
        }
        return nullptr;
    }

    static void writeMemory(void* address, void* buffer, size_t size) {
        DWORD oldProtect;
        VirtualProtectEx(handle, address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
        WriteProcessMemory(handle, address, buffer, size, nullptr);
        VirtualProtectEx(handle, address, size, oldProtect, &oldProtect);
    }

    static void readMemory(void* address, void* buffer, size_t size) {
        ReadProcessMemory(handle, address, buffer, size, nullptr);
    }

    static void* scanSignature(const std::string& signature) {
        return nullptr;
    }
};
