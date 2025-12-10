//
// Created by xande on 6/11/2025.
//

#include "Model.h"
#include "CubeData.h"

namespace D3D12Engine {
HRESULT Model::Initialize(ID3D12Device *device, ID3D12DeviceContext *device_context, const std::string& texture_filename) {
    // Load in the model data.
    RETURN_FAIL_IF_FAILED(LoadModel())

    // Initialize the vertex and index buffers.
    RETURN_FAIL_IF_FAILED(InitializeBuffers(device))

    RETURN_FAIL_IF_FAILED(LoadTexture(device, device_context, texture_filename))

    return S_OK;
}

void Model::Shutdown() {
}

void Model::Render(ID3D12DeviceContext *device_context) {
    RenderBuffers(device_context);
}

int Model::GetIndexCount() const {
    return m_indexCount;
}

ID3D12ShaderResourceView * Model::GetTexture() const {
    return m_texture->GetTexture();
}

HRESULT Model::LoadModel() {
    // Read in the vertex count.
    m_vertexCount = CubeData::vertex_size;

    // Set the number of indices to be the same as the vertex count.
    m_indexCount = m_vertexCount;

    // Create the model using the vertex count that was read in.
    m_model.resize(m_vertexCount);
    
    for (unsigned int i = 0; i < m_vertexCount; ++i) {
        // Position data
        m_model[i].x = CubeData::vertex_data[i * CubeData::stride];
        m_model[i].y = CubeData::vertex_data[i * CubeData::stride + 1];
        m_model[i].z = CubeData::vertex_data[i * CubeData::stride + 2];

        // Normal data
        m_model[i].nx = CubeData::vertex_data[i * CubeData::stride + 3];
        m_model[i].ny = CubeData::vertex_data[i * CubeData::stride + 4];
        m_model[i].nz = CubeData::vertex_data[i * CubeData::stride + 5];

        // UV data
        m_model[i].tu = CubeData::vertex_data[i * CubeData::stride + 6];
        m_model[i].tv = CubeData::vertex_data[i * CubeData::stride + 7];
    }
    return S_OK;
}

HRESULT Model::InitializeBuffers(ID3D12Device *device) {
    // Create the vertex array.
    VertexType* vertices = new VertexType[m_vertexCount];
    RETURN_FAIL_IF_NULL(vertices)

    // Create the index array.
    // unsigned long* indices = new unsigned long[m_indexCount];
    // RETURN_FAIL_IF_NULL(indices)

    // Load the vertex array and index array with data.
    for(unsigned int i = 0; i < m_vertexCount; ++i)
    {
        vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
        vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
        vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

        // indices[i] = i;
    }

    // Set up the description of the static vertex buffer.
    D3D12_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D12_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D12_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    D3D12_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now create the vertex buffer.
    RETURN_FAIL_IF_FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer))

    // Set up the description of the static index buffer.
    D3D12_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D12_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D12_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    D3D12_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = CubeData::index_data;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    RETURN_FAIL_IF_FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer))

    // Release the arrays now that the vertex and index buffers have been created and loaded.
    delete [] vertices;
    // delete [] indices;

    return S_OK;
}

void Model::RenderBuffers(ID3D12DeviceContext *device_context) {
    // Set vertex buffer stride and offset.
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    device_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    device_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    device_context->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

HRESULT Model::LoadTexture(ID3D12Device *device, ID3D12DeviceContext *device_context, const std::string &filename) {
    m_texture = std::make_unique<Texture>();
    RETURN_FAIL_IF_FAILED(m_texture->Initialize(device, device_context, filename))

    return S_OK;
}
} // D3D12Engine