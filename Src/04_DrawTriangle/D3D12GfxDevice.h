#pragma once

#include "D3D12GfxContext.h"

class D3D12GfxDevice
{
private:
    D3D12GfxContext* _gfxContext;

public:
    D3D12GfxDevice();
    ~D3D12GfxDevice();
    D3D12GfxContext* GetGfxContext();
};
