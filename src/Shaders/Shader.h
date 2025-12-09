//
// Created by xande on 6/11/2025.
//

#ifndef SHADER_H
#define SHADER_H

#include "pch.hpp"

namespace D3D11Engine {
class Shader {
protected:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };
    struct LightBufferType
    {
        XMFLOAT4 diffuseColor;
        XMFLOAT3 lightDirection;
        float time;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
    };
public:
    Shader() = default;
    Shader(const Shader&) = default;
    virtual ~Shader() = default;

protected:
    virtual HRESULT InitializeShader(ID3D11Device *device, HWND hwnd, std::wstring& shader_filename);
    virtual HRESULT SetShaderParameters(ID3D11DeviceContext *device_context, XMMATRIX world_matrix, XMMATRIX view_matrix,
                                XMMATRIX projection_matrix);
    virtual void RenderShader(ID3D11DeviceContext *device_context, int index_count);
    void OutputShaderErrorMessage(ID3D10Blob *error_message, HWND hwnd, std::wstring &shader_filename);

private:
    HRESULT UpdateTime(ID3D11DeviceContext *device_context);

protected:
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_layout;
    ComPtr<ID3D11Buffer> m_matrixBuffer;
    ComPtr<ID3D11Buffer> m_timeBuffer;
    ComPtr<ID3D11SamplerState> m_sampleState;
    ComPtr<ID3D11Buffer> m_lightBuffer;

};

} // D3D11Engine

#endif //SHADER_H
