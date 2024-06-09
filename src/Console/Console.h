#pragma once

#ifndef CONSOLE_H
#define CONSOLE_H

#include <cstdio>
#include <Windows.h>
#include <fstream>
//#define DISABLE_LOGGING_CONSOLE

#ifndef DISABLE_LOGGING_CONSOLE
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

namespace Console {
    inline FILE* f;

    static void Alloc() {
        AllocConsole();
        freopen_s(&f, "CONOUT$", "w", stdout);
        SetConsoleTitle(PROJECT_NAME" - Debug Console");
    }


    static bool Free() {
        if (GetConsoleWindow()) {
            fclose(f);
            FreeConsole();

            return true;
        }
        return false;
    }
}


#endif //CONSOLE_H
