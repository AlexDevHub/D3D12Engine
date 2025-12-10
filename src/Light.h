//
// Created by xande on 6/12/2025.
//

#ifndef LIGHT_H
#define LIGHT_H

#include "pch.hpp"
namespace D3D12Engine {
class Light {
public:
    Light() = default;
    Light(const Light&) = default;
    ~Light() = default;

    void SetDiffuseColor(float red, float green, float blue, float alpha);
    void SetDirection(float x, float y, float z);

    XMFLOAT4 GetDiffuseColor();
    XMFLOAT3 GetDirection();

private:
    XMFLOAT4 m_diffuseColor;
    XMFLOAT3 m_direction;
};

} // D3D12Engine

#endif //LIGHT_H
