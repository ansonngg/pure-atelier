#pragma once

// Precompiled header
#include "PCH.h"

// Local headers
#include "Clock.h"
#include "Controller.h"

namespace DirectXECS::Core {
class Window {
public:
    Window(HWND windowHandle, uint32_t clientWidth, uint32_t clientHeight, bool useVsync);

    void Start();
    void Destroy();

    void Update();
    void ToggleFullscreen();
    void Resize(uint32_t width, uint32_t height);

private:
    static constexpr UINT BufferCount = 2;

    HWND windowHandle_;
    uint32_t clientWidth_;
    uint32_t clientHeight_;
    bool isFullscreen_ = false;

    Microsoft::WRL::ComPtr<IDXGISwapChain4> dxgiSwapChain_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> d3d12RTVDescriptorHeap_;
    Microsoft::WRL::ComPtr<ID3D12Resource> d3d12BackBuffers_[BufferCount];
    uint64_t fenceValues_[BufferCount] = {};
    UINT rtvDescriptorSize_{};
    UINT currentBackBufferIndex_{};
    RECT windowRect_{};
    UINT syncInterval_{};
    UINT presentFlags_{};

    Clock clock_;
    Controller controller_;

    void CreateSwapChain_(bool isTearingSupported);
    void CreateDescriptorHeap_();
    void UpdatePresentArgs_(bool isTearingSupported, bool useVsync);
    void UpdateRenderTargetViews_();

    void Render_();
    void TransitionCurrentBackBuffer_(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> &commandList,
        D3D12_RESOURCE_STATES stateBefore,
        D3D12_RESOURCE_STATES stateAfter
    );
    void Clear_(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> &commandList);
    void Present_();

    static bool IsTearingSupported_();
};
}
