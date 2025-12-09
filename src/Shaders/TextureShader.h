//
// Created by xande on 6/11/2025.
//

#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "Shader.h"
#include "pch.hpp"

namespace D3D11Engine {

class TextureShader : Shader {
public:
    TextureShader() = default;
    TextureShader(const TextureShader&) = default;
    ~TextureShader() = default;

    HRESULT Initialize(ID3D11Device*, HWND);
    HRESULT Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
                              XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

private:
    HRESULT SetShaderParameters(ID3D11DeviceContext *device_context, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix, ID3D11ShaderResourceView* texture);
    void RenderShader(ID3D11DeviceContext *device_context, int index_count) override;

private:

};

} // D3D11Engine

#endif //TEXTURESHADER_H
