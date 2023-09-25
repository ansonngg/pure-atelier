#include "Window.h"

#include "Application.h"
#include "CommandQueue.h"
#include "Util/Helper.h"

using Microsoft::WRL::ComPtr;

namespace DirectXECS::Core {
Window::Window(HWND windowHandle, uint32_t clientWidth, uint32_t clientHeight, bool useVsync)
    : windowHandle_(windowHandle)
    , clientWidth_(clientWidth)
    , clientHeight_(clientHeight) {
    bool isTearingSupported = IsTearingSupported_();
    CreateSwapChain_(isTearingSupported);
    CreateDescriptorHeap_();
    UpdatePresentArgs_(isTearingSupported, useVsync);
    UpdateRenderTargetViews_();
}

void Window::Start() {
    ::ShowWindow(windowHandle_, SW_SHOW);
    clock_.Init();
}

void Window::Destroy() {
    ::DestroyWindow(windowHandle_);
    windowHandle_ = nullptr;
}

void Window::Update() {
    clock_.Tick();
    controller_.Update(clock_.GetDeltaSecond(), clock_.GetTotalSecond());
    Render_();
}

void Window::ToggleFullscreen() {
    isFullscreen_ = !isFullscreen_;

    if (isFullscreen_) {
        // Store the current window dimensions so that they can be restored
        ::GetWindowRect(windowHandle_, &windowRect_);

        // Set the window style to borderless windowed
        ::SetWindowLongW(
            windowHandle_,
            GWL_STYLE,
            WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
        );

        // Query the name of the nearest display device for the window
        HMONITOR hMonitor = ::MonitorFromWindow(windowHandle_, MONITOR_DEFAULTTONEAREST);
        MONITORINFOEX monitorInfo = {
            {
                .cbSize = sizeof(MONITORINFOEX),
            },
        };
        ::GetMonitorInfo(hMonitor, &monitorInfo);

        ::SetWindowPos(
            windowHandle_,
            HWND_TOPMOST,
            monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.top,
            monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE
        );

        ::ShowWindow(windowHandle_, SW_MAXIMIZE);
    } else {
        // Restore all the window decorators
        ::SetWindowLong(windowHandle_, GWL_STYLE, WS_OVERLAPPEDWINDOW);

        ::SetWindowPos(
            windowHandle_,
            HWND_NOTOPMOST,
            windowRect_.left,
            windowRect_.top,
            windowRect_.right - windowRect_.left,
            windowRect_.bottom - windowRect_.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE
        );

        ::ShowWindow(windowHandle_, SW_NORMAL);
    }
}

void Window::Resize(uint32_t width, uint32_t height) {
    if (width == clientWidth_ && height == clientHeight_) {
        return;
    }

    clientWidth_ = std::max(1u, width);
    clientHeight_ = std::max(1u, height);
    Application::GetInstance().Flush();

    for (auto &backBuffer : d3d12BackBuffers_) {
        backBuffer.Reset();
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    Util::ThrowIfFailed(dxgiSwapChain_->GetDesc(&swapChainDesc));
    Util::ThrowIfFailed(dxgiSwapChain_->ResizeBuffers(
        BufferCount,
        clientWidth_,
        clientHeight_,
        swapChainDesc.BufferDesc.Format,
        swapChainDesc.Flags
    ));

    currentBackBufferIndex_ = dxgiSwapChain_->GetCurrentBackBufferIndex();
    UpdateRenderTargetViews_();
}

void Window::CreateSwapChain_(bool isTearingSupported) {
    ComPtr<IDXGIFactory4> factory;
    UINT createFactoryFlags;

#ifdef _DEBUG
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#else
    createFactoryFlags = 0;
#endif // _DEBUG

    Util::ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory)));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
        .Width = clientWidth_,
        .Height = clientHeight_,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .Stereo = FALSE,
        .SampleDesc = { 1, 0 },
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = BufferCount,
        .Scaling = DXGI_SCALING_STRETCH,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags = (UINT)(isTearingSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0),
    };

    ID3D12CommandQueue *commandQueue =
        Application::GetInstance().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT)->GetD3D12CommandQueue().Get();
    ComPtr<IDXGISwapChain1> swapChain1;
    Util::ThrowIfFailed(
        factory->CreateSwapChainForHwnd(commandQueue, windowHandle_, &swapChainDesc, nullptr, nullptr, &swapChain1)
    );

    // Disable the Alt+Enter fullscreen toggle feature
    Util::ThrowIfFailed(factory->MakeWindowAssociation(windowHandle_, DXGI_MWA_NO_ALT_ENTER));

    Util::ThrowIfFailed(swapChain1.As(&dxgiSwapChain_));
    currentBackBufferIndex_ = dxgiSwapChain_->GetCurrentBackBufferIndex();
}

void Window::CreateDescriptorHeap_() {
    ComPtr<ID3D12Device2> device = Application::GetInstance().GetD3D12Device();
    D3D12_DESCRIPTOR_HEAP_DESC desc = {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = BufferCount,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0,
    };
    Util::ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&d3d12RTVDescriptorHeap_)));
    rtvDescriptorSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void Window::UpdatePresentArgs_(bool isTearingSupported, bool useVsync) {
    syncInterval_ = useVsync ? 1 : 0;
    presentFlags_ = isTearingSupported && !useVsync ? DXGI_PRESENT_ALLOW_TEARING : 0;
}

void Window::UpdateRenderTargetViews_() {
    ComPtr<ID3D12Device> device = Application::GetInstance().GetD3D12Device();
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(d3d12RTVDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());

    for (int i = 0; i < BufferCount; ++i) {
        ComPtr<ID3D12Resource> backBuffer;
        Util::ThrowIfFailed(dxgiSwapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));
        device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
        d3d12BackBuffers_[i] = backBuffer;
        rtvHandle.Offset((INT)rtvDescriptorSize_);
    }
}

void Window::Render_() {
    CommandQueue *commandQueue = Application::GetInstance().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    ComPtr<ID3D12GraphicsCommandList2> commandList = commandQueue->GetD3D12CommandList();
    TransitionCurrentBackBuffer_(commandList, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    Clear_(commandList);
    TransitionCurrentBackBuffer_(commandList, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    fenceValues_[currentBackBufferIndex_] = commandQueue->ExecuteCommandList(commandList);
    Present_();
    currentBackBufferIndex_ = dxgiSwapChain_->GetCurrentBackBufferIndex();
    commandQueue->WaitForFenceValue(fenceValues_[currentBackBufferIndex_]);
}

void Window::TransitionCurrentBackBuffer_(
    const ComPtr<ID3D12GraphicsCommandList2> &commandList,
    D3D12_RESOURCE_STATES stateBefore,
    D3D12_RESOURCE_STATES stateAfter
) {
    CD3DX12_RESOURCE_BARRIER barrier =
        CD3DX12_RESOURCE_BARRIER::Transition(d3d12BackBuffers_[currentBackBufferIndex_].Get(), stateBefore, stateAfter);
    commandList->ResourceBarrier(1, &barrier);
}

void Window::Clear_(const ComPtr<ID3D12GraphicsCommandList2> &commandList) {
    FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        d3d12RTVDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(),
        (INT)currentBackBufferIndex_,
        rtvDescriptorSize_
    );
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void Window::Present_() {
    Util::ThrowIfFailed(dxgiSwapChain_->Present(syncInterval_, presentFlags_));
}

bool Window::IsTearingSupported_() {
    BOOL allowTearing = FALSE;
    ComPtr<IDXGIFactory4> factory4;

    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4)))) {
        ComPtr<IDXGIFactory5> factory5;

        if (SUCCEEDED(factory4.As(&factory5))) {
            factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
        }
    }

    return allowTearing == TRUE;
}
}
