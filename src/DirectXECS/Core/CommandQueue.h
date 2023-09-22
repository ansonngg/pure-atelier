#pragma once

// System headers
#include "Util/WRLWrapper.h"
#include <cstdint>
#include <queue>
#include <utility>

// DirectX 12 headers
#include "directx/d3d12.h"

namespace DirectXECS::Core {
class CommandQueue {
public:
    explicit CommandQueue(D3D12_COMMAND_LIST_TYPE commandListType);

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue();
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> GetD3D12CommandList();

    uint64_t ExecuteCommandList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> &commandList);
    uint64_t Signal();
    bool IsFenceComplete(uint64_t fenceValue);
    void WaitForFenceValue(uint64_t fenceValue);
    void Flush();

private:
    D3D12_COMMAND_LIST_TYPE commandListType_;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> d3d12CommandQueue_;
    std::queue<std::pair<uint64_t, Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>> commandAllocatorQueue_;
    std::queue<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>> commandListQueue_;
    Microsoft::WRL::ComPtr<ID3D12Fence> d3d12Fence_;
    HANDLE fenceEvent_{};
    uint64_t fenceValue_ = 0;

    void CreateCommandQueue_();
    void CreateFence_();
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator_();
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>
    CreateCommandList_(const Microsoft::WRL::ComPtr<ID3D12CommandAllocator> &commandAllocator);
};
}
