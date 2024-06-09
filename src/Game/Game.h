#ifndef GAME_H
#define GAME_H
#include <memory>

#include "../Console/Console.h"

class Game {
public:
    Game(HMODULE hModule) {
        this->Initialize();
    }

    ~Game() {
        this->Uninitialize();
    }

    void Uninitialize();


    void UIControlsPolling();
private:
    void Initialize();
    static HWND g_hWindow;
};

inline std::unique_ptr<Game> pGame;

#endif //GAME_H
