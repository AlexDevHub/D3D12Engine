//
// Created by xande on 6/9/2025.
//

#ifndef SYSTEM_H
#define SYSTEM_H

#include "pch.hpp"

namespace D3D11Engine {
class System {
public:
    virtual ~System() = default;

    virtual HRESULT Init() = 0;
    virtual void Update() = 0;
    virtual HRESULT Shutdown() = 0;
};
} // namespace D3D11Engine



#endif //SYSTEM_H
