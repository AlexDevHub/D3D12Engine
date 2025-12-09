//
// Created by xande on 6/9/2025.
//

#ifndef D3D11API_H
#define D3D11API_H

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
#include <string>
#include <wrl.h>

#include "System.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace D3D11Engine {
class D3D11API : public System {
public:

    HRESULT Init() override;
    HRESULT Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
    void Update() override;
    HRESULT Shutdown() override;

    void BeginScene(float red, float green, float blue, float alpha);
    void EndScene();


    void GetProjectionMatrix(XMMATRIX &projectionMatrix) const;
    void GetWorldMatrix(XMMATRIX &worldMatrix) const;
    void GetOrthoMatrix(XMMATRIX &orthoMatrix) const;
    void GetVideoCardInfo(std::wstring& card_name, int &memory) const;

    void SetBackBufferRenderTarget();
    void ResetViewport();

    ID3D11DeviceContext* GetDeviceContext();
    ID3D11Device* GetDevice();

private:
    bool m_vsync_enabled = false;
    size_t m_videoCardMemory = 0;
    std::wstring m_videoCardDescription;
    ComPtr<IDXGISwapChain> m_swapChain = nullptr;
    ComPtr<ID3D11Device> m_device = nullptr;
    ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
    ComPtr<ID3D11Texture2D> m_depthStencilBuffer = nullptr;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState = nullptr;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView = nullptr;
    ComPtr<ID3D11RasterizerState> m_rasterState = nullptr;
    XMMATRIX m_projectionMatrix = {};
    XMMATRIX m_worldMatrix = {};
    XMMATRIX m_orthoMatrix = {};
    D3D11_VIEWPORT m_viewport = {};
};
}


#endif //D3D11API_H
