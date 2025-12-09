//
// Created by xande on 6/10/2025.
//

#include "ColorShader.h"

namespace D3D11Engine {
HRESULT ColorShader::Initialize(ID3D11Device *device, HWND hwnd) {

    std::wstring shader_filename (L"Assets/Shaders/color.hlsl");

    InitializeShader(device, hwnd, shader_filename);

    return S_OK;
}

HRESULT ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
                              XMMATRIX projectionMatrix) {
    RETURN_FAIL_IF_FAILED(SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
    RenderShader(deviceContext, indexCount);

    return S_OK;
}

HRESULT ColorShader::SetShaderParameters(ID3D11DeviceContext *device_context, XMMATRIX world_matrix,
    XMMATRIX view_matrix, XMMATRIX projection_matrix) {
    // Transpose the matrices to prepare them for the shader.
    world_matrix = XMMatrixTranspose(world_matrix);
    view_matrix = XMMatrixTranspose(view_matrix);
    projection_matrix = XMMatrixTranspose(projection_matrix);

    // Lock the constant buffer so it can be written to.
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    RETURN_FAIL_IF_FAILED(device_context->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))

    // Get a pointer to the data in the constant buffer.
    MatrixBufferType* dataPtr = static_cast<MatrixBufferType *>(mappedResource.pData);

    // Copy the matrices into the constant buffer.
    dataPtr->world = world_matrix;
    dataPtr->view = view_matrix;
    dataPtr->projection = projection_matrix;

    // Unlock the constant buffer.
    device_context->Unmap(m_matrixBuffer.Get(), 0);

    // Set the position of the constant buffer in the vertex shader.
    unsigned int bufferNumber = 0;

    // Finanly set the constant buffer in the vertex shader with the updated values.
    device_context->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

    return S_OK;
}

void ColorShader::RenderShader(ID3D11DeviceContext *device_context, int index_count) {
    // Set the vertex input layout.
    device_context->IASetInputLayout(m_layout.Get());

    // Set the vertex and pixel shaders that will be used to render this triangle.
    device_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    device_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Render the triangle.
    device_context->DrawIndexed(index_count, 0, 0);
}
} // D3D11Engine