//
// Created by xande on 6/11/2025.
//

#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "../pch.hpp"
#include "../Texture.h"

using namespace DirectX;

namespace D3D11Engine {
class Model {
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
        XMFLOAT2 texture;
        XMFLOAT3 normal;
    };

    struct ModelType
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

public:
    Model() = default;
    Model(const Model&) = default;
    ~Model() = default;

    HRESULT Initialize(ID3D11Device *device, ID3D11DeviceContext *device_context, const std::string& texture_filename);
    void Shutdown();
    void Render(ID3D11DeviceContext* device_context);

    int GetIndexCount() const;

    ID3D11ShaderResourceView* GetTexture() const;

private:
    HRESULT LoadModel();
    HRESULT InitializeBuffers(ID3D11Device *device);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext *device_context);

    HRESULT LoadTexture(ID3D11Device* device, ID3D11DeviceContext* device_context, const std::string& filename);
    void ReleaseTexture();

private:
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    int m_vertexCount, m_indexCount;
    std::unique_ptr<Texture> m_texture;
    std::vector<ModelType> m_model;

};

} // D3D11Engine

#endif //MODEL_H
