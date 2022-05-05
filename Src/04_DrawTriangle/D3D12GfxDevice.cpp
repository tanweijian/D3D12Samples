#include "D3D12GfxDevice.h"

D3D12GfxDevice::D3D12GfxDevice()
{
    _gfxContext = new D3D12GfxContext();
}

D3D12GfxDevice::~D3D12GfxDevice()
{
    delete _gfxContext;
    _gfxContext = nullptr;
}

D3D12GfxContext* D3D12GfxDevice::GetGfxContext()
{
    return _gfxContext;
}
