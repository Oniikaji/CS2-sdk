#include "Game.h"
#include "../Utility/Utility.h"
#include "../Hooks/Hooks.h"
#include "../Menu/Menu.h"

HWND Game::g_hWindow = nullptr;

void Game::Initialize() {
    Utility::InitProcessInfo();
    Hooks::Initialize();
}

void Game::Uninitialize() {
    Hooks::Uninitialize();
    Console::Free();
}

void Game::UIControlsPolling() {
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        Menu::open = !Menu::open;
    }
}




