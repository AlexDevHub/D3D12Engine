//
// Created by xande on 6/9/2025.
//

#ifndef ERROR_H
#define ERROR_H

#include <Winerror.h>

// From DXSampleHelper.h 
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}

#define RETURN_IF_FAILED(x)\
    {\
    auto res = (x);\
    if (FAILED(res)) return res;\
    }

#define RETURN_FALSE_IF_NULL(x)\
    if(!(x)) return false;

#define RETURN_FAIL_IF_NULL(x)\
    if(!(x)) return E_FAIL;

#define RETURN_FAIL_IF_FAILED(x)\
    if(FAILED(x)) return E_FAIL;

#define RETURN_VALUE_IF_FAILED(x, return_value)\
    if (FAILED(x)) return return_value;

#define RETURN_FALSE_IF_FAILED(x)\
    if (FAILED(x)) return false;

#endif //ERROR_H
