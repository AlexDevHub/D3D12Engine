//
// Created by xande on 6/11/2025.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace D3D12Engine {
Texture::~Texture() {
    if (m_targaData) {
        stbi_image_free(m_targaData);
        m_targaData = nullptr;
    }
}

HRESULT Texture::Initialize(ID3D12Device *device, ID3D12DeviceContext *device_context, const std::string &filename) {
    // Load the targa image data into memory.
    RETURN_FAIL_IF_FAILED(LoadTarga32Bit(filename))

    // Setup the description of the texture.
    D3D12_TEXTURE2D_DESC textureDesc;
    textureDesc.Height = m_height;
    textureDesc.Width = m_width;
    textureDesc.MipLevels = 0;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D12_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D12_BIND_SHADER_RESOURCE | D3D12_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D12_RESOURCE_MISC_GENERATE_MIPS;

    // Create the empty texture.
    RETURN_FAIL_IF_FAILED(device->CreateTexture2D(&textureDesc, nullptr, &m_texture))

    // Set the row pitch of the targa image data.
    unsigned int rowPitch = (m_width * 4) * sizeof(unsigned char);

    // Copy the targa image data into the texture.
    device_context->UpdateSubresource(m_texture.Get(), 0, nullptr, m_targaData, rowPitch, 0);

    // Setup the shader resource view description.
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    // Create the shader resource view for the texture.
    RETURN_FAIL_IF_FAILED(device->CreateShaderResourceView(m_texture.Get(), &srvDesc, &m_textureView))

    // Generate mipmaps for this texture.
    device_context->GenerateMips(m_textureView.Get());

    return S_OK;
}

ID3D12ShaderResourceView* Texture::GetTexture() {
    return m_textureView.Get();
}

int Texture::GetWidth() const {
    return m_width;
}

int Texture::GetHeight() const {
    return m_height;
}

HRESULT Texture::LoadTarga32Bit(const std::string &filename) {
    m_targaData = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 0);

    if (!m_targaData)
        return E_FAIL;

    return S_OK;
}
} // D3D12Engine