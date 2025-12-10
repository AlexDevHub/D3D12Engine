//
// Created by xande on 6/12/2025.
//

#include "Light.h"

namespace D3D12Engine {
void Light::SetDiffuseColor(float red, float green, float blue, float alpha) {
    m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDirection(float x, float y, float z) {
    m_direction = XMFLOAT3(x, y, z);
}

XMFLOAT4 Light::GetDiffuseColor() {
    return  m_diffuseColor;
}

XMFLOAT3 Light::GetDirection() {
    return m_direction;
}
} // D3D12Engine