//
// Created by xande on 6/9/2025.
//

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Utils/Error.h"

#include <string>
#include <memory>

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <directxmath.h>
#include <d3dcompiler.h>

// D3D12 extension library.
#include <d3dx12.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

#endif //PCH_H
