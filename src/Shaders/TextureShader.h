//
// Created by xande on 6/11/2025.
//

#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "Shader.h"
#include "pch.hpp"

namespace D3D12Engine {

class TextureShader : Shader {
public:
    TextureShader() = default;
    TextureShader(const TextureShader&) = default;
    ~TextureShader() = default;

    HRESULT Initialize(ID3D12Device*, HWND);
    HRESULT Render(ID3D12DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
                              XMMATRIX projectionMatrix, ID3D12ShaderResourceView* texture);

private:
    HRESULT SetShaderParameters(ID3D12DeviceContext *device_context, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix, ID3D12ShaderResourceView* texture);
    void RenderShader(ID3D12DeviceContext *device_context, int index_count) override;

private:

};

} // D3D12Engine

#endif //TEXTURESHADER_H
