//
// Created by xande on 6/9/2025.
//

#include "InputSystem.h"
namespace D3D11Engine {
HRESULT InputSystem::Init() {
    return S_OK;
}

HRESULT InputSystem::Init(GLFWwindow *window, Camera* camera) {
    glfwSetKeyCallback(window, key_processing);
    m_camera = camera;

    return S_OK;
}

void InputSystem::Update() {
    glfwPollEvents();

}

HRESULT InputSystem::Shutdown() {
    return S_OK;

}

void InputSystem::key_processing(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float mov_modifier = 100.0f * Application::dt;
    float rot_modifier = 1000.0f * Application::dt;

    auto pos = m_camera->GetPosition();
    auto rot = m_camera->GetRotation();

    // Forward
    if (key == GLFW_KEY_W) pos.z += mov_modifier;

    // Back
    if (key == GLFW_KEY_S) pos.z -= mov_modifier;

    // Left
    if (key == GLFW_KEY_A) pos.x -= mov_modifier;

    // Right
    if (key == GLFW_KEY_D) pos.x += mov_modifier;

    // Up
    if (key == GLFW_KEY_SPACE) pos.y += mov_modifier;

    // Down
    if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) pos.y -= mov_modifier;

    // Pitch Up
    if (key == GLFW_KEY_UP ) rot.x -= rot_modifier;

    // Pitch Down
    if (key == GLFW_KEY_DOWN ) rot.x += rot_modifier;

    // Yaw Left
    if (key == GLFW_KEY_LEFT ) rot.y -= rot_modifier;

    // Yaw Right
    if (key == GLFW_KEY_RIGHT ) rot.y += rot_modifier;

    // Roll Left
    if (key == GLFW_KEY_Q ) rot.z -= rot_modifier;

    // Roll Right
    if (key == GLFW_KEY_E ) rot.z += rot_modifier;

    m_camera->SetPosition(pos.x, pos.y, pos.z);
    m_camera->SetRotation(rot.x, rot.y, rot.z);

}
} // D3D11Engine