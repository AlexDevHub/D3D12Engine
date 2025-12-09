//
// Created by xande on 6/9/2025.
//

#include "Window.h"

D3D11Engine::Window::Window(const int width, const int height, std::string&& window_name) :
    m_width(width), m_height(height), m_window_name(std::move(window_name))
{
}

D3D11Engine::Window::~Window() {
}

HRESULT D3D11Engine::Window::Init() {
    m_window_handle = glfwCreateWindow(m_width, m_height,m_window_name.c_str(), nullptr, nullptr);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    return S_OK;
}

void D3D11Engine::Window::Update() {

}

HRESULT D3D11Engine::Window::Shutdown() {
    glfwDestroyWindow(m_window_handle);
    glfwTerminate();
    return S_OK;

}

bool D3D11Engine::Window::ShouldWindowClose() const {
    return glfwWindowShouldClose(m_window_handle);
}

HWND D3D11Engine::Window::getWindowHandleHWND() const {
    return glfwGetWin32Window(m_window_handle);
}

GLFWwindow* D3D11Engine::Window::getWindowHandle() const {
    return m_window_handle;
}

void D3D11Engine::Window::SetWindowShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(m_window_handle, shouldClose);
}

void D3D11Engine::Window::SetWindowName(std::string &&window_name) {
    glfwSetWindowTitle(m_window_handle, window_name.c_str());
}
