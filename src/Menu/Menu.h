#pragma once

#ifndef MENU_H
#define MENU_H

#include "imgui.h"


struct ImFont;

namespace Menu {
    inline int tabCount = 1;
    inline ImFont* normalText;
    inline ImFont* titleText;
    inline ImFont* highlightText;
    inline ImFont* espNameText;
    inline ImFont* subTitleText;
    inline ImFont* weaponIcons;


    inline float WIDTH = 970.f;
    inline float HEIGHT = 835.f;

    inline float widthSeparatorInt = WIDTH / 2;
    inline float heightSeparatorInt = HEIGHT / 2 + 20;

    inline bool open = true;
    inline ImVec2 menuSize{500, 500};

    inline struct Fonts {
        ImFont* roboto = nullptr;
    } fonts;

    void setStyle();

    void menuBar();

    void espRender();

    void aimRender();


    inline void render() {
        ImGui::PushFont(normalText);
        ImGui::SetNextWindowSize(menuSize);
        ImGui::Begin(
            PROJECT_NAME, nullptr,
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        menuBar();
        espRender();
        aimRender();

        ImGui::PopFont();
        ImGui::End();
    }
}
#endif //MENU_H
