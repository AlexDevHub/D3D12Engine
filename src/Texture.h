//
// Created by xande on 6/11/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

namespace D3D11Engine {

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

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* device_context, const std::string& filename);

    ID3D11ShaderResourceView* GetTexture();

    int GetWidth() const;
    int GetHeight() const;

private:
    HRESULT LoadTarga32Bit(const std::string& filename);

private:
    unsigned char* m_targaData = nullptr;
    ComPtr<ID3D11Texture2D> m_texture;
    ComPtr<ID3D11ShaderResourceView> m_textureView;
    int m_width = 0, m_height = 0, m_channels = 0;
};

} // D3D11Engine

#endif //TEXTURE_H
