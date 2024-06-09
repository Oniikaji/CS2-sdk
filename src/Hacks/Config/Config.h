#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include "../Esp.h"
#include "../Utility/Offsets.h"
#include "../Utility/Utility.h"

inline struct ESP {
    bool enable{false};
    bool box{false};
    bool name{false};
    bool healthBar{false};
    bool health{false};
} esp;

inline struct Misc {
    bool bunnyHop{false};
    bool POV{false};
} misc;

inline void CheckConfig() {

    // std::uintptr_t enginePointer = ReadMemory<std::uintptr_t>(ModuleBaseAddr::engine + Offsets::dwNetworkGameClient);

    // if (!IsValidPtr(enginePointer)) {
    //     std::cout << "Invalid Pointer\n";
    // } else {
    //     std::cout << enginePointer;
    // }


    // std::cout << gameState << std::endl;

    if (esp.enable)
        Esp::RenderEsp();
}

#endif //CONFIG_H
