//
// Created by xande on 6/11/2025.
//

#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

#include "pch.hpp"
#include "Shader.h"

namespace D3D11Engine {
class LightShader : Shader{

public:
    LightShader() = default;
    LightShader(const LightShader&) = default;
    ~LightShader() = default;

    HRESULT Initialize(ID3D11Device* device, HWND hwnd);
    HRESULT Render(ID3D11DeviceContext* device_context, int index_count, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix,
                              ID3D11ShaderResourceView* texture, XMFLOAT3 light_direction, XMFLOAT4 diffuse_color);

private:
    HRESULT SetShaderParameters(ID3D11DeviceContext* device_context, int index_count, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix,
                              ID3D11ShaderResourceView* texture, XMFLOAT3 light_direction, XMFLOAT4 diffuse_color);
    void RenderShader(ID3D11DeviceContext* device_context, int index_count);

private:
};

} // D3D11Engine

#endif //LIGHTSHADER_H
