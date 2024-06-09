#pragma once

#ifndef OFFSETS_H
#define OFFSETS_H

#include <windows.h>

namespace ModuleBaseAddr {
    inline uintptr_t client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));
    inline uintptr_t engine = reinterpret_cast<uintptr_t>(GetModuleHandle("engine2.dll"));
}


namespace Offsets {
    constexpr std::uintptr_t dwEntityList = 0x18C9E78;
    constexpr std::uintptr_t dwViewMatrix = 0x192B310;
    constexpr std::uintptr_t dwLocalPlayerController = 0x1919588;
    constexpr std::uintptr_t m_hPlayerPawn = 0x7E4;
    constexpr std::uintptr_t m_vOldOrigin = 0x127C;
    constexpr std::uintptr_t m_iHealth = 0x334;
    constexpr std::uintptr_t m_pGameSceneNode = 0x318;


    // engine2_dll
    // constexpr std::uintptr_t dwNetworkGameClient = 0x512AC8;
    // constexpr std::uintptr_t dwNetworkGameClient_signOnState = 0x240;
}

#endif //OFFSETS_H
