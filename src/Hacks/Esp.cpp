#include <map>

#include "Esp.h"

#include "../Utility/Offsets.h"

view_matrix_t GetGameViewMatrix() {
    uintptr_t viewMatrixAddress = ModuleBaseAddr::client + Offsets::dwViewMatrix;
    return *reinterpret_cast<view_matrix_t *>(viewMatrixAddress);
}

Vector3 Esp::WorldToScreen(Vector3* v) {
    view_matrix_t view_matrix = GetGameViewMatrix();

    float screenWidth = static_cast<float>(Utility::ProcessInfo::ClientWidth);
    float screenHeight = static_cast<float>(Utility::ProcessInfo::ClientHeight);

    float _x = view_matrix[0][0] * v->x + view_matrix[0][1] * v->y + view_matrix[0][2] * v->z + view_matrix[0][3];
    float _y = view_matrix[1][0] * v->x + view_matrix[1][1] * v->y + view_matrix[1][2] * v->z + view_matrix[1][3];

    float w = view_matrix[3][0] * v->x + view_matrix[3][1] * v->y + view_matrix[3][2] * v->z + view_matrix[3][3];

    if (w < 0.01f)
        return false;

    float inv_w = 1.f / w;
    _x *= inv_w;
    _y *= inv_w;

    float x = screenWidth * .5f;
    float y = screenHeight * .5f;

    x += 0.5f * _x * screenWidth + 0.5f;
    y -= 0.5f * _y * screenHeight + 0.5f;

    return {x, y, w};
}


std::vector<std::uintptr_t> Esp::GetGameEntities() {
    std::uintptr_t LocalPlayer = ReadMemory<uintptr_t>(ModuleBaseAddr::client + Offsets::dwLocalPlayerController);
    if (!IsValidPtr(LocalPlayer))
        return {0};


    std::uintptr_t LocalPlayerPawn = ReadMemory<uint32_t>(LocalPlayer + Offsets::m_hPlayerPawn);
    if (!LocalPlayerPawn)
        return {0};


    std::uintptr_t EntityList = ReadMemory<uintptr_t>(ModuleBaseAddr::client + Offsets::dwEntityList);
    if (!IsValidPtr(EntityList))
        return {0};


    std::uintptr_t lListEntry2 = ReadMemory<uintptr_t>(EntityList + 0x8 * ((LocalPlayerPawn & 0x7FFF) >> 9) + 16);
    if (!IsValidPtr(lListEntry2))
        return {0};


    std::uintptr_t LocalCSPlayerPawn = ReadMemory<uintptr_t>(lListEntry2 + 120 * (LocalPlayerPawn & 0x1FF));
    if (!IsValidPtr(LocalCSPlayerPawn))
        return {0};


    std::vector<std::uintptr_t> GameEntities;
    int i = 0;

    while (true) {
        i++;

        std::uintptr_t ListEntry = ReadMemory<uintptr_t>(EntityList + (8 * (i & 0x7FFF) >> 9) + 16);
        if (!IsValidPtr(ListEntry))
            break;


        std::uintptr_t Entity = ReadMemory<uintptr_t>(ListEntry + 120 * (i & 0x1FF));
        if (!IsValidPtr(Entity))
            continue;


        std::uintptr_t PlayerPawn = ReadMemory<uint32_t>(Entity + Offsets::m_hPlayerPawn);

        std::uintptr_t ListEntry2 = ReadMemory<uintptr_t>(EntityList + 0x8 * ((PlayerPawn & 0x7FFF) >> 9) + 16);
        if (!IsValidPtr(ListEntry2))
            continue;


        std::uintptr_t CSPlayerPawn = ReadMemory<uintptr_t>(ListEntry2 + 120 * (PlayerPawn & 0x1FF));
        if (!IsValidPtr(CSPlayerPawn))
            continue;


        if (CSPlayerPawn == LocalCSPlayerPawn)
            continue;

        int CSPlayerHealth = ReadMemory<int>(CSPlayerPawn + Offsets::m_iHealth);

        if (CSPlayerHealth > 100 || CSPlayerHealth <= 0)
            continue;

        GameEntities.push_back(CSPlayerPawn);
    }

    return GameEntities;
}

Vector3 GetPlayerPawnHead(std::uintptr_t CSPlayerPawn) {
    std::uintptr_t CSGameSceneNode = ReadMemory<uintptr_t>(CSPlayerPawn + Offsets::m_pGameSceneNode);

    if (!IsValidPtr(CSGameSceneNode))
        return {};

    std::uintptr_t CSBoneArray = ReadMemory<uintptr_t>(CSGameSceneNode + 0x160 + 0x80);

    if (!IsValidPtr(CSBoneArray))
        return {};

    std::uintptr_t BoneAddress = CSBoneArray + 6 * 32;

    return ReadMemory<Vector3>(BoneAddress);
}

std::vector<std::map<std::string, Vector3>> Esp::GetEntiesBonePositions(std::vector<std::uintptr_t> Entities) {

    std::vector<std::map<std::string, Vector3>> EntitiesBoneMap;

    for (std::uintptr_t Entity : Entities) {
        Vector3 Origin = ReadMemory<Vector3>(Entity + Offsets::m_vOldOrigin);
        Vector3 scOrigin = WorldToScreen(&Origin);

        if (scOrigin.z >= 0.01f) {
            std::uintptr_t CSGameSceneNode = ReadMemory<uintptr_t>(Entity + Offsets::m_pGameSceneNode);

            if (!IsValidPtr(CSGameSceneNode))
                return EntitiesBoneMap;

            std::uintptr_t CSBoneArray = ReadMemory<uintptr_t>(CSGameSceneNode + 0x160 + 0x80);

            if (!IsValidPtr(CSBoneArray))
                return EntitiesBoneMap;

            std::map<std::string, Vector3> cBones;

            for (const auto& Entry: BoneMap) {
                std::uintptr_t BoneAddress = CSBoneArray + Entry.second * 32;
                Vector3 cBonePosition = ReadMemory<Vector3>(BoneAddress);
                cBones[Entry.first] = cBonePosition;
            }

            EntitiesBoneMap.push_back(cBones);
        }
    }

    return EntitiesBoneMap;
}
