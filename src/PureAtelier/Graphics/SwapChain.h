#pragma once

namespace PureAtelier
{
class SwapChain
{
public:
    void ResizeBuffers(uint32_t width, uint32_t height);

private:
    static constexpr UINT BUFFER_COUNT = 2;

    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_DXGISwapChain;
};
}
