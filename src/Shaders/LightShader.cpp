//
// Created by xande on 6/11/2025.
//

#include "LightShader.h"
#include "../Application.h"
namespace D3D11Engine {
HRESULT LightShader::Initialize(ID3D11Device *device, HWND hwnd) {
    std::wstring shader_filename (L"Assets/Shaders/light.hlsl");
    RETURN_FAIL_IF_FAILED(Shader::InitializeShader(device, hwnd, shader_filename))

    return S_OK;
}

HRESULT LightShader::Render(ID3D11DeviceContext *device_context, int index_count, XMMATRIX world_matrix,
    XMMATRIX view_matrix, XMMATRIX projection_matrix, ID3D11ShaderResourceView *texture, XMFLOAT3 light_direction,
    XMFLOAT4 diffuse_color) {
    RETURN_FAIL_IF_FAILED(
        SetShaderParameters(
            device_context,
            index_count,
            world_matrix,
            view_matrix,
            projection_matrix,
            texture,
            light_direction,
            diffuse_color))

    RenderShader(device_context, index_count);

    return S_OK;
}

HRESULT LightShader::SetShaderParameters(ID3D11DeviceContext *device_context, int index_count, XMMATRIX world_matrix,
    XMMATRIX view_matrix, XMMATRIX projection_matrix, ID3D11ShaderResourceView *texture, XMFLOAT3 light_direction,
    XMFLOAT4 diffuse_color) {
    RETURN_FAIL_IF_FAILED(Shader::SetShaderParameters(device_context, world_matrix, view_matrix, projection_matrix))

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    // Set shader texture resource in the pixel shader.
    device_context->PSSetShaderResources(0, 1, &texture);

    // Lock the light constant buffer so it can be written to.
    RETURN_FAIL_IF_FAILED(device_context->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))

    // Get a pointer to the data in the constant buffer.
    LightBufferType *dataPtr2 = static_cast<LightBufferType *>(mappedResource.pData);

    // Copy the lighting variables into the constant buffer.
    dataPtr2->diffuseColor = diffuse_color;
    dataPtr2->lightDirection = light_direction;
    dataPtr2->time = Application::time;

    // Unlock the constant buffer.
    device_context->Unmap(m_lightBuffer.Get(), 0);

    // Set the position of the light constant buffer in the pixel shader.
    unsigned int bufferNumber = 0;

    // Finally set the light constant buffer in the pixel shader with the updated values.
    device_context->PSSetConstantBuffers(bufferNumber, 1, m_lightBuffer.GetAddressOf());

    return S_OK;
}

void LightShader::RenderShader(ID3D11DeviceContext *device_context, int index_count) {
    Shader::RenderShader(device_context, index_count);
}
} // D3D11Engine