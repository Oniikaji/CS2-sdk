#pragma once

#ifndef UTILITY_H
#define UTILITY_H
#include <Psapi.h>
#include <thread>
#include <windows.h>
#include "../Console/Console.h"

template<typename T>
T ReadMemory(uintptr_t address) {
    return *reinterpret_cast<T *>(address);
}

template <typename T>
bool IsValidPtr(T p)
{
    return !IsBadReadPtr((void*)p, sizeof(T));
}

namespace Utility {
    inline std::uint64_t LookupSignatureInstance(char* Pattern, char* Mask, std::uint64_t Base, size_t Size)
    {
        size_t PatternSize = strlen((char*)Mask);

        for (int i = 0; i < Size; i++)
        {
            bool Match = true;
            for (int j = 0; j < PatternSize; j++)
            {
                if (*(char*)((uintptr_t)Base + i + j) != Pattern[j] && Mask[j] != '?')
                {
                    Match = false;
                    break;
                }
            }
            if (Match) return (Base + i);
        }
        return 0;
    }

    static BOOL __stdcall EnumWindowsCallback(HWND handle, LPARAM lParam) {
    const auto isMainWindow = [handle]() {
        return GetWindow(handle, GW_OWNER) == nullptr && IsWindowVisible(handle);
    };

    DWORD pID = 0;
    GetWindowThreadProcessId(handle, &pID);

    if (GetCurrentProcessId() != pID || !isMainWindow() || handle == GetConsoleWindow())
        return true;

    *reinterpret_cast<HWND*>(lParam) = handle;

    return false;  // Stop enumeration after finding the main window
}

// Function to get the main window of the current process
inline HWND GetProcessWindow() {
    HWND hwnd = nullptr;
    EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&hwnd));

    while (!hwnd) {
        EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&hwnd));
        LOG("[!] Waiting for window to appear.\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    char name[128];
    GetWindowTextA(hwnd, name, RTL_NUMBER_OF(name));
    LOG("[+] Got window with name: '%s'\n", name);

    return hwnd;
}

namespace ProcessInfo {
    inline DWORD ID;
    inline HANDLE Handle;
    inline HWND Hwnd;
    inline int ClientWidth;
    inline int ClientHeight;
    inline std::string Title;
    inline std::string ClassName;
    inline std::string Path;
}

inline void InitProcessInfo() {
    ProcessInfo::Hwnd = GetProcessWindow();
    ProcessInfo::ID = GetCurrentProcessId();
    ProcessInfo::Handle = GetCurrentProcess();

    RECT clientRect;
    GetClientRect(ProcessInfo::Hwnd, &clientRect);
    ProcessInfo::ClientWidth = clientRect.right - clientRect.left;
    ProcessInfo::ClientHeight = clientRect.bottom - clientRect.top;

    char tempTitle[MAX_PATH];
    GetWindowText(ProcessInfo::Hwnd, tempTitle, sizeof(tempTitle));
    ProcessInfo::Title = tempTitle;

    char tempClassName[MAX_PATH];
    GetClassName(ProcessInfo::Hwnd, tempClassName, sizeof(tempClassName));
    ProcessInfo::ClassName = tempClassName;

    char tempPath[MAX_PATH];
    GetModuleFileNameEx(ProcessInfo::Handle, NULL, tempPath, sizeof(tempPath));
    ProcessInfo::Path = tempPath;
}
}



#endif //UTILITY_H
