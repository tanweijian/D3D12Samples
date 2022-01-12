#include <d3d12.h>
#include <dxgi1_6.h>

#include "Include/LogAssert.h"

int main(int argc, char** argv)
{
    UINT dxgiFactoryFlags = 0;
    HRESULT hr = E_FAIL;

    IDXGIFactory7* _factory = nullptr;
    IDXGIAdapter1* _adpater = nullptr;
    ID3D12Device8* _device = nullptr;

#if defined(_DEBUG)
    ID3D12Debug3* debugController = nullptr;
    hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
    ASSERT_SUCCEEDED(hr, "d3d12: failed create debug layer");
    debugController->EnableDebugLayer();
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

    hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&_factory));
    ASSERT_SUCCEEDED(hr, "d3d12: failed create dxgi factory");

    DXGI_ADAPTER_DESC1 desc = {};
    D3D12_FEATURE_DATA_ARCHITECTURE1 architecture = {};
    for (UINT adapterIndex = 0; _factory->EnumAdapters1(adapterIndex, &_adpater) != DXGI_ERROR_NOT_FOUND; adapterIndex++)
    {
        hr = _adpater->GetDesc1(&desc);
        if (SUCCEEDED(hr))
        {
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }
            LOGDEBUG(L"显卡[%d] \"%s\" 独占显存[%dMB] 独占内存[%dMB] 共享内存[%dMB]", adapterIndex, desc.Description, desc.DedicatedVideoMemory / (1024 * 1024), desc.DedicatedSystemMemory / (1024 * 1024), desc.SharedSystemMemory / (1024 * 1024));

            hr = D3D12CreateDevice(_adpater, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&_device));
            if (SUCCEEDED(hr))
            {
                hr = _device->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE1, &architecture, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE1));
                if (SUCCEEDED(hr))
                {
                    if (architecture.UMA)
                    {
                        if (architecture.CacheCoherentUMA)
                        {
                            LOGDEBUG("CC-UMA 架构");
                        }
                        else
                        {
                            LOGDEBUG("UMA 架构");
                        }
                    }
                    else if (architecture.IsolatedMMU)
                    {
                        LOGDEBUG("NUMA 架构");
                    }
                    
                    if (architecture.TileBasedRenderer)
                    {
                        LOGDEBUG("支持 tile based renderer");
                    }
                }
            }
        }
    }

    return 0;
}