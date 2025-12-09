#include "pch.hpp"
#include "Application.h"
#include "Utils/Error.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
    auto app = std::make_unique<D3D11Engine::Application>();
    RETURN_IF_FAILED(app->Init())

    app->Update();

    RETURN_IF_FAILED(app->Shutdown())

    return 0;
}
