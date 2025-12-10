//
// Created by xande on 6/11/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "pch.hpp"

namespace D3D12Engine {

class Texture {
private:
    struct TargaHeader
    {
        unsigned char data1[12];
        unsigned short width;
        unsigned short height;
        unsigned char bpp;
        unsigned char data2;
    };

public:
    Texture() = default;
    Texture(const Texture&) = default;
    ~Texture();

    HRESULT Initialize(ID3D12Device* device, ID3D12DeviceContext* device_context, const std::string& filename);

    ID3D12ShaderResourceView* GetTexture();

    int GetWidth() const;
    int GetHeight() const;

private:
    HRESULT LoadTarga32Bit(const std::string& filename);

private:
    unsigned char* m_targaData = nullptr;
    ComPtr<ID3D12Texture2D> m_texture;
    ComPtr<ID3D12ShaderResourceView> m_textureView;
    int m_width = 0, m_height = 0, m_channels = 0;
};

} // D3D12Engine

#endif //TEXTURE_H
