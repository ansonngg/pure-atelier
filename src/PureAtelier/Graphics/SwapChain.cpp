#include "SwapChain.h"

#include "Utility/Exception.h"

namespace PureAtelier
{
void SwapChain::ResizeBuffers(uint32_t width, uint32_t height)
{
    for (auto& backBuffer : d3d12BackBuffers_)
    {
        backBuffer.Reset();
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ThrowIfFailed(m_DXGISwapChain->GetDesc(&swapChainDesc));
    ThrowIfFailed(
        m_DXGISwapChain->ResizeBuffers(
            BUFFER_COUNT,
            width,
            height,
            swapChainDesc.BufferDesc.Format,
            swapChainDesc.Flags
        )
    );

    currentBackBufferIndex_ = m_DXGISwapChain->GetCurrentBackBufferIndex();
    UpdateRenderTargetViews_();
}
}
