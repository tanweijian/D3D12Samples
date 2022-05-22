#include "D3D12GfxDevice.h"

bool D3D12GfxDevice::Initialize(UINT width, UINT height, HWND hwnd)
{
    HRESULT hr = E_FAIL;
    UINT dxgiFactoryFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    ComPtr<ID3D12Debug> debugLayer;
    hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
    if (FAILED(hr))
    {
        return false;
    }
    debugLayer->EnableDebugLayer();
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

    ComPtr<IDXGIFactory5> dxgiFactory;
    hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(hr))
    {
        return false;
    }

    ComPtr<IDXGIAdapter1> dxgiAdapter;
    hr = GetHardwareAdapter(dxgiFactory.Get(), &dxgiAdapter, true);
    if (SUCCEEDED(hr))
    {
        hr = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&mDevice));
    }
    else
    {
        hr = dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter));
        if (FAILED(hr))
        {
            return false;
        }
        hr = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&mDevice));
        if (FAILED(hr))
        {
            return false;
        }
    }

    hr = mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
    if (FAILED(hr))
    {
        return false;
    }
    _rtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    _dsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    _cvb_srv_uavDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    hr = mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));
    if (FAILED(hr))
    {
        return false;
    }
    hr = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));
    if (FAILED(hr))
    {
        return false;
    }
    hr = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mCommandList));
    if (FAILED(hr))
    {
        return false;
    }
    hr = mCommandList->Close();
    if (FAILED(hr))
    {
        return false;
    }

    hr = CreateSwapChain(dxgiFactory.Get(), width, height, hwnd);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

HRESULT D3D12GfxDevice::GetHardwareAdapter(IDXGIFactory5* factory, IDXGIAdapter1** ppAdapter, bool highPerformance)
{
    HRESULT hr = E_FAIL;
    ComPtr<IDXGIAdapter1> adapter;
    ComPtr<IDXGIFactory6> factory6;
    hr = factory->QueryInterface(IID_PPV_ARGS(&factory6));
    if (SUCCEEDED(hr))
    {
        UINT adapterIndex = 0;
        DXGI_GPU_PREFERENCE preference = highPerformance ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED;
        for (;;)
        {
            hr = factory6->EnumAdapterByGpuPreference(adapterIndex, preference, IID_PPV_ARGS(&adapter));
            if (FAILED(hr))
            {
                break;
            }
            adapterIndex++;
            DXGI_ADAPTER_DESC1 desc;
            hr = adapter->GetDesc1(&desc);
            if (FAILED(hr))
            {
                continue;
            }
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }
            hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_1, __uuidof(ID3D12Device), nullptr);
            if (SUCCEEDED(hr))
            {
                *ppAdapter = adapter.Detach();
                break;
            }
        }
    }
    else
    {
        UINT adapterIndex = 0;
        for (;;)
        {
            hr = factory->EnumAdapters1(adapterIndex, &adapter);
            if (FAILED(hr))
            {
                break;
            }
            adapterIndex++;
            DXGI_ADAPTER_DESC1 desc;
            hr = adapter->GetDesc1(&desc);
            if (FAILED(hr))
            {
                continue;
            }
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }
            hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_1, __uuidof(ID3D12Device), nullptr);
            if (SUCCEEDED(hr))
            {
                *ppAdapter = adapter.Detach();
                break;
            }
        }
    }

    return hr;
}

HRESULT D3D12GfxDevice::CreateSwapChain(IDXGIFactory5* factory, UINT width, UINT height, HWND hwnd)
{
    if (mSwapChain != nullptr)
    {
        mSwapChain.Reset();
    }
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    HRESULT hr = E_FAIL;
    hr = factory->CreateSwapChainForHwnd(mCommandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &mSwapChain);
    return hr;
}
