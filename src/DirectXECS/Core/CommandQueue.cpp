#include "CommandQueue.h"

// Local headers
#include "Application.h"
#include "Util/Helper.h"

using Microsoft::WRL::ComPtr;

namespace DirectXECS::Core {
CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE commandListType) : commandListType_(commandListType) {
    CreateCommandQueue_();
    CreateFence_();
}

ComPtr<ID3D12CommandQueue> CommandQueue::GetD3D12CommandQueue() {
    return d3d12CommandQueue_;
}

ComPtr<ID3D12GraphicsCommandList2> CommandQueue::GetD3D12CommandList() {
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    ComPtr<ID3D12GraphicsCommandList2> commandList;

    if (!commandAllocatorQueue_.empty() && IsFenceComplete(commandAllocatorQueue_.front().first)) {
        commandAllocator = commandAllocatorQueue_.front().second;
        commandAllocatorQueue_.pop();
        Util::ThrowIfFailed(commandAllocator->Reset());
    } else {
        commandAllocator = CreateCommandAllocator_();
    }

    if (!commandListQueue_.empty()) {
        commandList = commandListQueue_.front();
        commandListQueue_.pop();
        Util::ThrowIfFailed(commandList->Reset(commandAllocator.Get(), nullptr));
    } else {
        commandList = CreateCommandList_(commandAllocator);
    }

    // Associate the command allocator with the command list
    Util::ThrowIfFailed(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));
    return commandList;
}

uint64_t CommandQueue::ExecuteCommandList(const ComPtr<ID3D12GraphicsCommandList2> &commandList) {
    commandList->Close();

    ID3D12CommandAllocator *commandAllocator;
    UINT dataSize = sizeof(commandAllocator);
    Util::ThrowIfFailed(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

    ID3D12CommandList *const commandLists[] = { commandList.Get() };
    d3d12CommandQueue_->ExecuteCommandLists(1, commandLists);
    uint64_t fenceValue = Signal();

    commandAllocatorQueue_.emplace(fenceValue, commandAllocator);
    commandListQueue_.push(commandList);
    commandAllocator->Release();

    return fenceValue;
}

uint64_t CommandQueue::Signal() {
    d3d12CommandQueue_->Signal(d3d12Fence_.Get(), ++fenceValue_);
    return fenceValue_;
}

bool CommandQueue::IsFenceComplete(uint64_t fenceValue) {
    return d3d12Fence_->GetCompletedValue() >= fenceValue;
}

void CommandQueue::WaitForFenceValue(uint64_t fenceValue) {
    if (!IsFenceComplete(fenceValue)) {
        d3d12Fence_->SetEventOnCompletion(fenceValue, fenceEvent_);
        ::WaitForSingleObject(fenceEvent_, DWORD_MAX);
    }
}

void CommandQueue::Flush() {
    WaitForFenceValue(Signal());
}

void CommandQueue::CreateCommandQueue_() {
    D3D12_COMMAND_QUEUE_DESC desc = {
        .Type = commandListType_,
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0,
    };
    Util::ThrowIfFailed(
        Application::GetInstance().GetD3D12Device()->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue_))
    );
}

void CommandQueue::CreateFence_() {
    Util::ThrowIfFailed(Application::GetInstance().GetD3D12Device()->CreateFence(
        fenceValue_,
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(&d3d12Fence_)
    ));
    fenceEvent_ = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
    assert(fenceEvent_ && "Failed to create fence event handle.");
}

ComPtr<ID3D12CommandAllocator> CommandQueue::CreateCommandAllocator_() {
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    Util::ThrowIfFailed(Application::GetInstance().GetD3D12Device()->CreateCommandAllocator(
        commandListType_,
        IID_PPV_ARGS(&commandAllocator)
    ));
    return commandAllocator;
}

ComPtr<ID3D12GraphicsCommandList2>
CommandQueue::CreateCommandList_(const ComPtr<ID3D12CommandAllocator> &commandAllocator) {
    ComPtr<ID3D12GraphicsCommandList2> commandList;
    Util::ThrowIfFailed(
        Application::GetInstance()
            .GetD3D12Device()
            ->CreateCommandList(0, commandListType_, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList))
    );
    return commandList;
}
}
