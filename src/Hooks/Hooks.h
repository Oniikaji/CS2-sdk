#ifndef HOOKS_H
#define HOOKS_H

#include <d3d11.h>
#include <dxgi.h>


namespace Hooks {
    typedef HRESULT (__stdcall*ResizeBuffers)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height,
                                              DXGI_FORMAT NewFormat, UINT SwapChainFlags);
    typedef HRESULT (__stdcall*Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    typedef void(__stdcall *DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);

    inline DrawIndexed oDrawIndexed;
    inline Present oPresent;
    inline ResizeBuffers oResizeBuffers;

    void __stdcall hkDrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
    long __stdcall hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height,
                                          DXGI_FORMAT NewFormat, UINT SwapChainFlags);

    long __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

    bool Initialize();
    bool Uninitialize();
}



#endif //HOOKS_H
