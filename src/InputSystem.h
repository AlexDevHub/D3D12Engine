//
// Created by xande on 6/9/2025.
//

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "Application.h"
#include "System.h"
#include "Camera.h"
#include "pch.hpp"

namespace D3D11Engine {

class InputSystem : System {
public:
    HRESULT Init() override;
    HRESULT Init(GLFWwindow* window, Camera* camera);
    void Update() override;
    HRESULT Shutdown() override;

private:
    static void key_processing(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
    inline static Camera* m_camera = nullptr;
};

} // D3D11Engine

#endif //INPUTSYSTEM_H
