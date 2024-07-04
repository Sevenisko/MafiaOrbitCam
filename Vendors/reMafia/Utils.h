#pragma once
#include <cstdio>
#include "common.h"

static char g_szTempBuf[1024] = {};

template <typename T>
T clamp(T x, T min, T max) {
    if (x < min)
        x = min;
    if (x > max)
        x = max;
    return x;
}


typedef NTSTATUS(WINAPI* RtlGetVersion_Proc)(LPOSVERSIONINFOEXW);
inline const char* WinVerName(OSVERSIONINFOEXW* info) {
    switch (info->dwMajorVersion) {
    case 10:
        if (info->wProductType != VER_NT_WORKSTATION) {
            if (info->dwBuildNumber > 17763)
                return "Windows Server";
            else if (info->dwBuildNumber > 14393)
                return "Windows Server 2019";
            else
                return "Windows Server 2016";
        } else {
            if (info->dwBuildNumber < 20000)
                return "Windows 10";
            else
                return "Windows 11";
        }

    case 6:
        switch (info->dwMinorVersion) {
        case 3:
            if (info->wProductType != VER_NT_WORKSTATION)
                return "Windows Server 2012 R2";
            else
                return "Windows 8.1";
        case 2:
            if (info->wProductType != VER_NT_WORKSTATION)
                return "Windows Server 2012";
            else
                return "Windows 8";
        case 1:
            if (info->wProductType != VER_NT_WORKSTATION)
                return "Windows Server 2008 R2";
            else
                return "Windows 7";
        case 0:
            if (info->wProductType != VER_NT_WORKSTATION)
                return "Windows Server 2008";
            else
                return "Windows Vista";
        }
    case 5:
        switch (info->dwMinorVersion) {
        case 2:
            if (GetSystemMetrics(89) != 0)
                return "Windows Server 2003 R2";
            else if (GetSystemMetrics(89) == 0)
                return "Windows Server 2003";
            else
                return "Windows XP x64 Edition";
        case 1: return "Windows XP";
        case 0: return "Windows 2000";
        }

    default: return "Unknown Windows";
    }
}

inline void GetWinVersion(OSVERSIONINFOEXW* info) {
    ZeroMemory(info, sizeof(OSVERSIONINFOEXW));
    info->dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
    HMODULE hNTDll = GetModuleHandleA("ntdll.dll");
    RtlGetVersion_Proc RtlGetVersion = (RtlGetVersion_Proc)GetProcAddress(hNTDll, "RtlGetVersion");
    if (RtlGetVersion != NULL) {
        RtlGetVersion(info);
    }
}