//
// Created by xande on 6/11/2025.
//

#include "Camera.h"

namespace D3D11Engine {
void Camera::SetPosition(float x, float y, float z) {
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
}

void Camera::SetRotation(float x, float y, float z) {
    m_rotationX = x;
    m_rotationY = y;
    m_rotationZ = z;
}

XMFLOAT3 Camera::GetPosition() {
    return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 Camera::GetRotation() {
    return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::Render() {
    // Setup the vector that points upwards.
    XMFLOAT3 up;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    // Load it into a XMVECTOR structure.
    XMVECTOR upVector = XMLoadFloat3(&up);

    // Setup the position of the camera in the world.
    XMFLOAT3 position;
    position.x = m_positionX;
    position.y = m_positionY;
    position.z = m_positionZ;

    // Load it into a XMVECTOR structure.
    XMVECTOR positionVector = XMLoadFloat3(&position);

    // Setup where the camera is looking by default.
    XMFLOAT3 lookAt;
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;

    // Load it into a XMVECTOR structure.
    XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
    float pitch = m_rotationX * 0.0174532925f;
    float yaw   = m_rotationY * 0.0174532925f;
    float roll  = m_rotationZ * 0.0174532925f;

    // Create the rotation matrix from the yaw, pitch, and roll values.
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
    upVector = XMVector3TransformCoord(upVector, rotationMatrix);

    // Translate the rotated camera position to the location of the viewer.
    lookAtVector = XMVectorAdd(positionVector, lookAtVector);

    // Finally create the view matrix from the three updated vectors.
    m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(XMMATRIX &view_matrix) const {
    view_matrix = m_viewMatrix;

}
} // D3D11Engine
