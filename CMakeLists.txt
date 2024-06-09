cmake_minimum_required(VERSION 3.28)
project(Alysscheat)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:ICF")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MFC_FLAG 2)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")

#set(PKG_CONFIG_EXECUTABLE "D:/Msys2/mingw64/bin/pkg-config.exe")
#find_package(PkgConfig)

find_library(D3D11_LIB d3d11)
find_library(DXGI_LIB dxgi)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c" "src/*.h")

set(THIRD_DIR ${PROJECT_SOURCE_DIR}/ext)

set(ADDITIONAL_SUBDIRECTORIES "Console" "Hooks" "Timer" "Menu" "Game" "Hacks" "Utility")

foreach(subdir ${ADDITIONAL_SUBDIRECTORIES})
    file(GLOB_RECURSE SRC_SUBDIR "src/${subdir}/*.cpp" "src/${subdir}/*.c" "src/${subdir}/*.h")
    list(APPEND SRC ${SRC_SUBDIR})
endforeach()

include_directories(
        ${THIRD_DIR}/imgui
        ${THIRD_DIR}/minhook
)


add_subdirectory(${THIRD_DIR})
add_library(${PROJECT_NAME} SHARED ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE MINHOOK_LIB IMGUI_LIB ${D3D11_LIB} ${DXGI_LIB} d3dcompiler)


