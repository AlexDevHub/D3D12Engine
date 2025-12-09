//
// Created by xande on 6/11/2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "pch.hpp"
#include <directxmath.h>

using namespace DirectX;

namespace D3D11Engine {

class Camera {
public:
    Camera() = default;
    Camera(const Camera&) = default;
    ~Camera() = default;

    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);

    XMFLOAT3 GetPosition();
    XMFLOAT3 GetRotation();

    void Render();
    void GetViewMatrix(XMMATRIX& view_matrix) const;

private:
    float m_positionX = 0.0f, m_positionY = 0.0f, m_positionZ = 0.0f;
    float m_rotationX = 0.0f, m_rotationY = 0.0f, m_rotationZ = 0.0f;
    XMMATRIX m_viewMatrix = {};
};

} // D3D11Engine

#endif //CAMERA_H
