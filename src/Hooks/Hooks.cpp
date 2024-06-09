#pragma once

#include "Hooks.h"

#include <d3d11.h>
#include <psapi.h>
#include <MinHook.h>

#include <imgui_impl_dx11.h>
#include "imgui_impl_win32.h"
#include <iostream>

#include "../Hacks/Config/Config.h"
#include "../Menu/Menu.h"
#include "../Utility/Utility.h"

bool init;

HWND WindowHwnd;

namespace DirectXInterface {
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    ID3D11RenderTargetView* renderTargetView;
}

WNDPROC originalWndProc;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (Menu::open) {
        ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
        return true;
    }
    return CallWindowProc(originalWndProc, hwnd, uMsg, wParam, lParam);
}

long __stdcall Hooks::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (!init) {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DirectXInterface::device))) {
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            (void) io;
            ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

            DirectXInterface::device->GetImmediateContext(&DirectXInterface::context);

            DXGI_SWAP_CHAIN_DESC Desc;
            pSwapChain->GetDesc(&Desc);
            WindowHwnd = Desc.OutputWindow;

            ID3D11Texture2D* BackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &BackBuffer);
            DirectXInterface::device->CreateRenderTargetView(BackBuffer, nullptr, &DirectXInterface::renderTargetView);
            BackBuffer->Release();

            io.Fonts->AddFontDefault();
            Menu::normalText = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 15.f);
            Menu::titleText = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 16.f);
            Menu::subTitleText = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 15.f);
            Menu::highlightText = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanai.ttf", 13.f);
            Menu::espNameText = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 15.f);
            Menu::weaponIcons = io.Fonts->AddFontFromFileTTF("E:\\CLionprojects\\Alysscheat\\src\\Font\\weaponIcons.ttf",
            23.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

            ImGui::StyleColorsDark();

            ImGui_ImplWin32_Init(WindowHwnd);
            ImGui_ImplDX11_Init(DirectXInterface::device, DirectXInterface::context);
            ImGui_ImplDX11_CreateDeviceObjects();
            ImGui::GetIO().ImeWindowHandle = WindowHwnd;
            originalWndProc = (WNDPROC) SetWindowLongPtr(WindowHwnd, GWLP_WNDPROC, (__int3264) (LONG_PTR) WndProc);

            Menu::setStyle();
            init = true;
        }
    }
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::GetIO().MouseDrawCursor = Menu::open;

    if (Menu::open == true) {
        Menu::render();
    }
    CheckConfig();
    ImGui::Render();
    DirectXInterface::context->OMSetRenderTargets(1, &DirectXInterface::renderTargetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
}

long __stdcall Hooks::hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height,
                                      DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
    if (DirectXInterface::renderTargetView) {
        DirectXInterface::context->OMSetRenderTargets(0, 0, 0);
        DirectXInterface::renderTargetView->Release();
    }

    HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

    ID3D11Texture2D* pBuffer;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **) &pBuffer);
    // Perform error handling here!
    DirectXInterface::device->CreateRenderTargetView(pBuffer, NULL, &DirectXInterface::renderTargetView);
    // Perform error handling here!
    pBuffer->Release();

    DirectXInterface::context->OMSetRenderTargets(1, &DirectXInterface::renderTargetView, NULL);

    // Set up the viewport.
    D3D11_VIEWPORT vp;
    vp.Width = Width;
    vp.Height = Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    DirectXInterface::context->RSSetViewports(1, &vp);
    return hr;
}


bool Hooks::Initialize() {
    MODULEINFO dxgi = {};
    K32GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("dxgi.dll"), &dxgi, sizeof(MODULEINFO));

    char SignaturePresent[] = "\xe9\x00\x00\x00\x00\x48\x89\x74\x24\x00\x55";
    char MaskPresent[] = "x????xxxx?x";

    unsigned __int64 SignatureResultPresent = Utility::LookupSignatureInstance
    (
        SignaturePresent, MaskPresent, (unsigned __int64) dxgi.lpBaseOfDll,
        (unsigned __int64) dxgi.lpBaseOfDll + (unsigned __int64) dxgi.SizeOfImage
    );

    if (SignatureResultPresent != NULL) {
        MH_Initialize();

        PVOID Original = nullptr;
        MH_STATUS status = MH_CreateHook((void *) SignatureResultPresent, &hkPresent, &Original);

        if (status == MH_OK) {
            *(void **) &oPresent = Original;

            if (MH_EnableHook((void *) SignatureResultPresent) == MH_OK) {
                std::cout << "Hooked Present function at address: " << std::hex << SignatureResultPresent << std::endl;
            }
        }
    }

    char SignatureResizebuffer[] = "\xe9\x00\x00\x00\x00\x54\x41\x55\x41\x56\x41";;
    char MaskResizebuffer[] = "x????xxxxxx";

    unsigned __int64 SignatureResizeResult = Utility::LookupSignatureInstance(
        SignatureResizebuffer, MaskResizebuffer, (unsigned __int64) dxgi.
        lpBaseOfDll,
        (unsigned __int64) dxgi.lpBaseOfDll + (unsigned __int64) dxgi.
        SizeOfImage);


    if (SignatureResizeResult != NULL) {
        PVOID Original = nullptr;
        MH_STATUS status = MH_CreateHook((void *) SignatureResizeResult, &hkResizeBuffers, &Original);

        if (status == MH_STATUS::MH_OK) {
            *(void **) &oResizeBuffers = Original;

            if (MH_EnableHook((void *) SignatureResizeResult) == MH_STATUS::MH_OK) {
                std::cout << "Hooked Resize function at address: " << std::hex << SignatureResizeResult << std::endl;
                return true;
            }
        }
        return true;
    }
    return true;
}

bool Hooks::Uninitialize() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    return true;
}
