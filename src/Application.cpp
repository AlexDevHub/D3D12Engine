//
// Created by xande on 6/9/2025.
//

#include "Application.h"

#include "Timer.h"

constexpr bool FULLSCREEN = false;
constexpr bool VSYNC_ENABLED = false;
constexpr float SCREEN_DEPTH = 1000.0f;
constexpr float SCREEN_NEAR = 0.3f;

HRESULT D3D12Engine::Application::Init() {
    if (!glfwInit()) {
        return E_FAIL;
    }

    m_window = std::make_unique<Window>(640, 480, "D3D12Engine");
    RETURN_FAIL_IF_FAILED(m_window->Init())
    HWND window_hwnd = m_window->getWindowHandleHWND();

    m_D3D12api = std::make_unique<D3D12API>();
    RETURN_FAIL_IF_FAILED(m_D3D12api->Init(640, 480, VSYNC_ENABLED, window_hwnd, false, SCREEN_DEPTH, SCREEN_NEAR))

    m_camera = std::make_unique<Camera>();
    m_camera->SetPosition(0.0f, 0.0f, -5.0f);

    m_input = std::make_unique<InputSystem>();
    m_input->Init(m_window->getWindowHandle(), m_camera.get());

    std::string texture_filename("Assets/Textures/stone01.tga");
    m_model = std::make_unique<Model>();
    RETURN_FAIL_IF_FAILED(m_model->Initialize(m_D3D12api->GetDevice(),m_D3D12api->GetDeviceContext(), texture_filename))

    // Create and initialize the light object.
    m_light = std::make_unique<Light>();

    m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_light->SetDirection(0.0f, 0.0f, 1.0f);

    m_color_shader = std::make_unique<ColorShader>();
    RETURN_FAIL_IF_FAILED(m_color_shader->Initialize(m_D3D12api->GetDevice(), window_hwnd))

    // Create and initialize the texture shader object.
    m_texture_shader = std::make_unique<TextureShader>();

    if(FAILED(m_texture_shader->Initialize(m_D3D12api->GetDevice(), window_hwnd)))
    {
        MessageBoxW(window_hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
        return E_FAIL;
    }

    // Create and initialize the light shader object.
    m_light_shader = std::make_unique<LightShader>();
    if(FAILED(m_light_shader->Initialize(m_D3D12api->GetDevice(), window_hwnd)))
    {
        MessageBoxW(window_hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
        return E_FAIL;
    }

    return S_OK;
}

void D3D12Engine::Application::Update() {
    unsigned int frame_counter = 0;
    float accumulated_time = 0.0f;
    float last_time = 0.f;
    Timer timer{};
    while (!m_window->ShouldWindowClose()) {
        time = static_cast<float>(glfwGetTime());
        dt = time - last_time;
        last_time = time;

        m_input->Update();

        // auto now = std::chrono::high_resolution_clock::now();
        timer.Reset();
        if (FAILED(Frame())) {
            m_window->SetWindowShouldClose(true);
        }
        ms = timer.ElapsedMillis();
        accumulated_time += timer.Elapsed();
        ++frame_counter;

        if (accumulated_time >= 1.0f) {
            fps = frame_counter;

            std::string new_window_name = std::format("D3D12Engine  FPS: {}   ms: {}ms ", fps, ms);
            m_window->SetWindowName(std::move(new_window_name));

            frame_counter = 0;
            accumulated_time = 0.f;

        }
    }
}

HRESULT D3D12Engine::Application::Shutdown() {
    glfwTerminate();

    return S_OK;
}

HRESULT D3D12Engine::Application::Frame() {
    static float rotation = 0.0f;

    rotation -= 0.3f * Application::dt;
    if(rotation < 0.0f) rotation += 360.0f;

    RETURN_FAIL_IF_FAILED(Render(rotation))

    return S_OK;
}

HRESULT D3D12Engine::Application::Render(float rotation) {
    // Clear the buffers to begin the scene.
    m_D3D12api->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate the view matrix based on the camera's position.
    m_camera->Render();

    // Get the world, view, and projection matrices from the camera and d3d objects.
    XMMATRIX worldMatrix{}, viewMatrix{}, projectionMatrix{};
    m_D3D12api->GetWorldMatrix(worldMatrix);
    m_camera->GetViewMatrix(viewMatrix);
    m_D3D12api->GetProjectionMatrix(projectionMatrix);

    // Rotate the world matrix by the rotation value so that the triangle will spin.
    worldMatrix = XMMatrixRotationY(rotation);

    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
    m_model->Render(m_D3D12api->GetDeviceContext());

    // Render the model using the light shader.
    RETURN_FAIL_IF_FAILED(m_light_shader->Render(m_D3D12api->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture(),
                               m_light->GetDirection(), m_light->GetDiffuseColor()))

    // Render the model using the texture shader.
    // RETURN_FAIL_IF_FAILED(m_texture_shader->Render(m_D3D12api->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture()))

    // Render the model using the color shader.
    // RETURN_FAIL_IF_FAILED(m_color_shader->Render(m_D3D12api->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))

    // Present the rendered scene to the screen.
    m_D3D12api->EndScene();

    return S_OK;
}

