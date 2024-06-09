#ifndef ESP_H
#define ESP_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <windows.h>

#include "imgui.h"
#include "../Utility/Utility.h"
#include "../Utility/Vector.h"
// #include "Config/Config.h"

inline std::vector<std::pair<std::string, std::string>> BoneConnections = {
    {"neck_0", "spine_1"},
    {"spine_1", "spine_2"},
    {"spine_2", "pelvis"},
    {"spine_1", "arm_upper_L"},
    {"arm_upper_L", "arm_lower_L"},
    {"arm_lower_L", "hand_L"},
    {"spine_1", "arm_upper_R"},
    {"arm_upper_R", "arm_lower_R"},
    {"arm_lower_R", "hand_R"},
    {"pelvis", "leg_upper_L"},
    {"leg_upper_L", "leg_lower_L"},
    {"leg_lower_L", "ankle_L"},
    {"pelvis", "leg_upper_R"},
    {"leg_upper_R", "leg_lower_R"},
    {"leg_lower_R", "ankle_R"}
};

inline std::map<std::string, int> BoneMap = {
    {"head", 6},
    {"neck_0", 5},
    {"spine_1", 4},
    {"spine_2", 2},
    {"pelvis", 0},
    {"arm_upper_L", 8},
    {"arm_lower_L", 9},
    {"hand_L", 10},
    {"arm_upper_R", 13},
    {"arm_lower_R", 14},
    {"hand_R", 15},
    {"leg_upper_L", 22},
    {"leg_lower_L", 23},
    {"ankle_L", 24},
    {"leg_upper_R", 25},
    {"leg_lower_R", 26},
    {"ankle_R", 27}
};

namespace Esp {
    std::vector<std::uintptr_t> GetGameEntities();

    std::vector<std::map<std::string, Vector3>> GetEntiesBonePositions(std::vector<std::uintptr_t> Entities);

    Vector3 WorldToScreen(Vector3* v);

    inline void RenderEsp() {
        ImDrawList* pDrawList = ImGui::GetForegroundDrawList();

        std::vector<std::uintptr_t> GameEntities = GetGameEntities();
        std::vector<std::map<std::string, Vector3>> Mapping = GetEntiesBonePositions(GameEntities);

        for (std::map<std::string, Vector3> Map: Mapping) {
            for (const auto& Connection: BoneConnections) {
                Vector3 vBoneFrom = WorldToScreen(&Map[Connection.first]);
                Vector3 vBoneTo = WorldToScreen(&Map[Connection.second]);

                if (vBoneFrom.z >= 0.01f && vBoneTo.z >= 0.01f) {
                    pDrawList->AddLine(ImVec2(vBoneFrom.x, vBoneFrom.y), ImVec2(vBoneTo.x, vBoneTo.y),
                                       ImColor(224, 224, 224), 1.5f);
                }
            }
        }
    }
}


#endif //HOOKS_H
