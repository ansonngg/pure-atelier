/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

namespace PureAtelier
{
class DynamicDescriptorHeapAllocation;
class FreeList;

struct StaleDescriptor
{
    uint32_t Offset;
    uint32_t DescriptorCount;
    uint64_t FrameNum;

    StaleDescriptor(uint32_t offset, uint32_t descriptorCount, uint64_t frameNum);
};

class DynamicDescriptorHeapPage : public std::enable_shared_from_this<DynamicDescriptorHeapPage>
{
public:
    DynamicDescriptorHeapPage(D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t descriptorCount);

    [[nodiscard]] uint32_t GetFreeHandleCount() const;

    DynamicDescriptorHeapAllocation Allocate(uint32_t descriptorCount);
    void Free(DynamicDescriptorHeapAllocation&& allocation);
    void ReleaseStaleDescriptors();

private:
    D3D12_DESCRIPTOR_HEAP_TYPE m_HeapType;
    uint32_t m_DescriptorCountInHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_D3D12DescriptorHeap;
    CD3DX12_CPU_DESCRIPTOR_HANDLE m_BaseDescriptor{};
    uint32_t m_DescriptorHandleIncrementSize;
    uint32_t m_FreeHandleCount;
    std::unique_ptr<FreeList> m_FreeList;
    std::queue<StaleDescriptor> m_StaleDescriptorQueue;
};
}
