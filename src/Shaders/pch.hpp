//
// Created by xande on 6/9/2025.
//
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Utils/Error.h"

#include <string>
#include <memory>

#include <directxmath.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;
