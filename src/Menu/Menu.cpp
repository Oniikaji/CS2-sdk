#include "Menu.h"
#include "../Hacks/Config/Config.h"
#include <imgui_impl_win32.h>

struct Vector3;

void Menu::setStyle() {
    ImGuiStyle* style = &ImGui::GetStyle();

    style->FramePadding = ImVec2(5, 5);
    style->FrameBorderSize = 1.f;
    style->FrameRounding = 2.f;

    style->WindowPadding = ImVec2(6, 6);

    style->GrabRounding = 0.f;
    style->GrabMinSize = 20.f;

    style->ButtonTextAlign = ImVec2(0.5, 0.5);

    style->ItemSpacing = ImVec2(9, 4);

    ImColor darkGrey = ImColor(29, 31, 31, 255);
    ImColor lightGrey = ImColor(38, 42, 43, 255);
    ImColor lightGreyTrans = ImColor(38, 42, 43, 175);

    ImColor newDarkpink = ImColor(171, 0, 60, 255);
    ImColor newLightpink = ImColor(219, 0, 76, 255);

    // Colours
    style->Colors[ImGuiCol_FrameBg] = lightGrey;
    style->Colors[ImGuiCol_FrameBgHovered] = darkGrey;
    style->Colors[ImGuiCol_FrameBgActive] = darkGrey;

    style->Colors[ImGuiCol_TitleBgActive] = newDarkpink;
    style->Colors[ImGuiCol_TitleBgCollapsed] = lightGreyTrans;

    style->Colors[ImGuiCol_ChildBg] = darkGrey;

    style->Colors[ImGuiCol_MenuBarBg] = lightGrey;
    style->Colors[ImGuiCol_WindowBg] = lightGrey;

    style->Colors[ImGuiCol_CheckMark] = newDarkpink;

    style->Colors[ImGuiCol_Button] = newDarkpink;
    style->Colors[ImGuiCol_ButtonHovered] = newLightpink;

    style->Colors[ImGuiCol_SliderGrab] = newDarkpink;
    style->Colors[ImGuiCol_SliderGrabActive] = newLightpink;

    style->Colors[ImGuiCol_ResizeGrip] = newDarkpink;
    style->Colors[ImGuiCol_ResizeGripHovered] = newLightpink;

    style->Colors[ImGuiCol_HeaderHovered] = newDarkpink;
    style->Colors[ImGuiCol_HeaderActive] = newLightpink;
}

void Menu::espRender() {
    if (tabCount == 1) {
        ImGui::BeginChild("Features", ImVec2(widthSeparatorInt, heightSeparatorInt), true);
        ImGui::PushFont(titleText);
        ImGui::Text("Esp - Features");
        ImGui::PopFont();
        ImGui::Spacing();
        ImGui::Checkbox("Enable ESP", &esp.enable);
        ImGui::Spacing();
        ImGui::Checkbox("Health Bar", &esp.healthBar);
        ImGui::Spacing();
        ImGui::EndChild();
    }
}

void Menu::aimRender() {
    if (tabCount == 2) {
        ImGui::BeginChild("Features", ImVec2(widthSeparatorInt, heightSeparatorInt), true);
        ImGui::PushFont(titleText);
        ImGui::Text("Aim - Features");
        ImGui::PopFont();
        ImGui::Spacing();
        ImGui::Checkbox("Aimbot", &esp.enable);
        ImGui::EndChild();
    }
}

void Menu::menuBar() {
    ImGui::BeginMenuBar();

    if (ImGui::MenuItem("ESP")) tabCount = 1;
    if (ImGui::MenuItem("Aim")) tabCount = 2;

    ImGui::EndMenuBar();
}
