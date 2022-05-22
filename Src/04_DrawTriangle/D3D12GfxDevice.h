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
    ComPtr<ID3D12Fence> mFence;
    ComPtr<ID3D12Device> mDevice;
    ComPtr<IDXGISwapChain1> mSwapChain;
    ComPtr<ID3D12CommandQueue> mCommandQueue;
    ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> mCommandList;

    bool Initialize(UINT width, UINT height, HWND hwnd);
    HRESULT GetHardwareAdapter(IDXGIFactory5* factory, IDXGIAdapter1** ppAdapter, bool highPerformance);
    HRESULT CreateSwapChain(IDXGIFactory5* factory, UINT width, UINT height, HWND hwnd);
};
