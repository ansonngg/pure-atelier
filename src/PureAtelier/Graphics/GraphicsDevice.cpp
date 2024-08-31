/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "GraphicsDevice.h"

#include "Utility/Exception.h"

namespace PureAtelier
{
using Microsoft::WRL::ComPtr;

GraphicsDevice& GraphicsDevice::GetInstance()
{
    static GraphicsDevice graphicsDevice;
    return graphicsDevice;
}

void GraphicsDevice::Initialize()
{
    auto adapter = GetAdapter();
    ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device)));

    // Enable debug messages in debug mode.
#ifdef _DEBUG
    ComPtr<ID3D12InfoQueue> infoQueue;

    if (SUCCEEDED(m_D3D12Device.As(&infoQueue)))
    {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

        // Suppress messages based on their severity level
        D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };

        // Suppress individual messages by their ID
        D3D12_MESSAGE_ID denyIds[] = {
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
        };

        D3D12_INFO_QUEUE_FILTER newFilter = {
            .DenyList = {
                .NumSeverities = _countof(severities),
                .pSeverityList = severities,
                .NumIDs = _countof(denyIds),
                .pIDList = denyIds,
            },
        };

        ThrowIfFailed(infoQueue->PushStorageFilter(&newFilter));
    }
#endif // _DEBUG
}

void GraphicsDevice::CreateCommittedResource(std::size_t bufferSize, ComPtr<ID3D12Resource>& resource)
{
    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    ThrowIfFailed(
        m_D3D12Device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resource)
        )
    );
}

ComPtr<IDXGIAdapter4> GraphicsDevice::GetAdapter()
{
    ComPtr<IDXGIFactory4> factory;
    UINT createFactoryFlags;

#ifdef _DEBUG
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#else
    createFactoryFlags = 0;
#endif // _DEBUG

    ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory)));

    ComPtr<IDXGIAdapter1> adapter1;
    ComPtr<IDXGIAdapter4> adapter4;
    SIZE_T maxDedicatedVideoMemory = 0;

    for (UINT i = 0; factory->EnumAdapters1(i, &adapter1) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC1 adapterDesc;
        adapter1->GetDesc1(&adapterDesc);

        // Pick the adapter with the largest dedicated video memory
        if ((adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0
            && SUCCEEDED(D3D12CreateDevice(adapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))
            && adapterDesc.DedicatedVideoMemory > maxDedicatedVideoMemory)
        {
            maxDedicatedVideoMemory = adapterDesc.DedicatedVideoMemory;
            ThrowIfFailed(adapter1.As(&adapter4));
        }
    }

    return adapter4;
}
}
