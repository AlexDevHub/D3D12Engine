//
// Created by xande on 6/9/2025.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "System.h"

namespace D3D11Engine {
class Window : public System{
public:
    Window(const int width, const int height, std::string&& window_name);
    ~Window() override;
    HRESULT Init() override;
    void Update() override;
    HRESULT Shutdown() override;

    bool ShouldWindowClose() const;

    HWND getWindowHandleHWND() const;
    GLFWwindow* getWindowHandle() const;
    void SetWindowShouldClose(bool shouldClose);
    void SetWindowName(std::string&& window_name);

private:
    int m_width = 0;
    int m_height = 0;
    std::string m_window_name;
    GLFWwindow *m_window_handle = nullptr;
};
} // namespace D3D11Engine



#endif //WINDOW_H
