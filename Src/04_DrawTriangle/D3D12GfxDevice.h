#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class D3D12GfxDevice
{
private:
    UINT _rtvDescriptorSize;
    UINT _dsvDescriptorSize;
    UINT _cvb_srv_uavDescriptorSize;

public:
    ComPtr<ID3D12Device> mDevice;
    ComPtr<ID3D12Fence> mFence;

    bool Initialize();
    HRESULT GetHardwareAdapter(IDXGIFactory5* factory, IDXGIAdapter1** ppAdapter, bool highPerformance);
};
