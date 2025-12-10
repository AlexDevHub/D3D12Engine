//
// Created by xande on 6/11/2025.
//

#include "TextureShader.h"

namespace D3D12Engine {
HRESULT TextureShader::Initialize(ID3D12Device* device, HWND hwnd) {
    std::wstring shader_filename (L"Assets/Shaders/texture.hlsl");

    InitializeShader(device, hwnd, shader_filename);
    return S_OK;
}

HRESULT TextureShader::Render(ID3D12DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix,
    XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D12ShaderResourceView *texture) {
    RETURN_FAIL_IF_FAILED(SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture))

    RenderShader(deviceContext, indexCount);

    return S_OK;
}

HRESULT TextureShader::SetShaderParameters(ID3D12DeviceContext *device_context, XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix, ID3D12ShaderResourceView* texture) {
    // Initialize matrix
    Shader::SetShaderParameters(device_context, world_matrix, view_matrix, projection_matrix);

    // Set shader texture resource in the pixel shader.
    device_context->PSSetShaderResources(0, 1, &texture);
    return S_OK;
}

void TextureShader::RenderShader(ID3D12DeviceContext *device_context, int index_count) {

    // Set the vertex input layout.
    device_context->IASetInputLayout(m_layout.Get());

    // Set the vertex and pixel shaders that will be used to render this triangle.
    device_context->VSSetShader(m_vertexShader.Get(), NULL, 0);
    device_context->PSSetShader(m_pixelShader.Get(), NULL, 0);

    // Set the sampler state in the pixel shader.
    device_context->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());

    // Render the triangle.
    device_context->DrawIndexed(index_count, 0, 0);
}
} // D3D12Engine