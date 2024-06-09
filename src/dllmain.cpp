#include "Game/Game.h"
#include "Timer/Timer.h"

void __stdcall MainThread(HMODULE hModule) {
    Console::Alloc();
    pGame = std::make_unique<Game>(hModule);

    EngineUtils::Timer* pTimer = EngineUtils::Timer::Instance();

    float frameRate = 144.0f;
    while (!(GetAsyncKeyState(VK_END) & 1)) {
        pTimer->Tick();
        if (pTimer->DeltaTime() >= 1 / frameRate) {
            pTimer->Reset();


            pGame->UIControlsPolling();
        }
    }
    pGame->Uninitialize();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            if (1) {
                HANDLE hThread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, nullptr);
                if (hThread != INVALID_HANDLE_VALUE)
                    CloseHandle(hThread);
            }
            break;
        case DLL_PROCESS_DETACH:
            // DISABLEALL()
            FreeLibraryAndExitThread(hModule, TRUE);
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        default:
            break;
    }
    return TRUE;
}
