//
// Created by xande on 6/10/2025.
//

#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "pch.hpp"
#include <fstream>
#include "Shader.h"

using namespace DirectX;

namespace D3D11Engine {
class ColorShader : Shader{
public:
    ColorShader() = default;
    ColorShader(const ColorShader&) = default;
    ~ColorShader() = default;

    HRESULT Initialize(ID3D11Device* device, HWND hwnd);
    HRESULT Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
    HRESULT SetShaderParameters(ID3D11DeviceContext *device_context, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix) override;
    void RenderShader(ID3D11DeviceContext *device_context, int index_count) override;

};
} // D3D11Engine

#endif //COLORSHADER_H
