//
// Created by xande on 6/9/2025.
//

#ifndef APPLICATION_H
#define APPLICATION_H

// #include "pch.hpp"
#include "D3D12API.h"
#include "InputSystem.h"
#include "System.h"
#include "Window.h"
#include "Camera.h"
#include "Models/Model.h"
#include "Shaders/ColorShader.h"
#include "Shaders/TextureShader.h"
#include "Shaders/LightShader.h"
#include "Light.h"

namespace D3D12Engine {
class Application : public  System{
public:
    Application() = default;
    ~Application() override = default;

    HRESULT Init() override;
    void Update() override;
    HRESULT Shutdown() override;

    HRESULT Frame();
    HRESULT Render(float rotation);

    friend class InputSystem;

public:
    inline static unsigned int fps = 0;
    inline static float ms = 0;
    inline static float time = 0;
    inline static float dt = 0;

private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<InputSystem> m_input;
    std::unique_ptr<D3D12API> m_D3D12api;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Model> m_model;
    std::unique_ptr<ColorShader> m_color_shader;
    std::unique_ptr<TextureShader> m_texture_shader;
    std::unique_ptr<LightShader> m_light_shader;
    std::unique_ptr<Light> m_light;
};
} // namespace D3D12Engine


#endif //APPLICATION_H
