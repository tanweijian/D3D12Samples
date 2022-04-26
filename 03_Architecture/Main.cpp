#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "spdlog/spdlog.h"
#include "Include/LogAssert.h"
#include "Include/Utils.h"

using namespace Microsoft::WRL;

int main(int argc, char** argv)
{
    UINT dxgiFactoryFlags = 0;
    HRESULT hr = E_FAIL;

#if defined(_DEBUG)
    ComPtr<ID3D12Debug3> debug;
    hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
    ASSERT_SUCCEEDED(hr, "d3d12: failed create debug layer");
    debug->EnableDebugLayer();
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

    ComPtr<IDXGIFactory7> factory;
    hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
    ASSERT_SUCCEEDED(hr, "d3d12: failed create dxgi factory");

    DXGI_ADAPTER_DESC1 desc = {};
    D3D12_FEATURE_DATA_ARCHITECTURE1 architecture = {};
    ComPtr<IDXGIAdapter1> adpater;
    for (UINT adapterIndex = 0; factory->EnumAdapters1(adapterIndex, &adpater) != DXGI_ERROR_NOT_FOUND; adapterIndex++)
    {
        hr = adpater->GetDesc1(&desc);
        if (SUCCEEDED(hr))
        {
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }
            SPDLOG_INFO("[{}] \"{}\" 独占显存[{}MB] 独占内存[{}MB] 共享内存[{}MB]", adapterIndex, wchar2char(desc.Description).get(), desc.DedicatedVideoMemory / (1024 * 1024), desc.DedicatedSystemMemory / (1024 * 1024), desc.SharedSystemMemory / (1024 * 1024));
            ComPtr<ID3D12Device8> device;
            hr = D3D12CreateDevice(adpater.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device));
            if (SUCCEEDED(hr))
            {
                hr = device->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE1, &architecture, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE1));
                if (SUCCEEDED(hr))
                {
                    if (architecture.UMA)
                    {
                        if (architecture.CacheCoherentUMA)
                        {
                            SPDLOG_INFO("CC-UMA 架构");
                        }
                        else
                        {
                            SPDLOG_INFO("UMA 架构");
                        }
                    }
                    else if (architecture.IsolatedMMU)
                    {
                        SPDLOG_INFO("NUMA 架构");
                    }

                    if (architecture.TileBasedRenderer)
                    {
                        SPDLOG_INFO("支持 tile based renderer");
                    }
                }
            }
        }
    }

    return 0;
}