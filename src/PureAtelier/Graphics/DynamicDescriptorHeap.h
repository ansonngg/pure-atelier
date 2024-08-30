#pragma once

namespace PureAtelier
{
class DynamicDescriptorHeap
{
public:
    DynamicDescriptorHeap(
        D3D12_DESCRIPTOR_HEAP_TYPE heapType,
        uint32_t unstagedDescriptorCountPerHeap = 256,
        uint32_t stagedDescriptorCountPerHeap = 1024
    );

private:
    D3D12_DESCRIPTOR_HEAP_TYPE m_HeapType;
    uint32_t m_UnstagedDescriptorCountPerHeap;
    uint32_t m_StagedDescriptorCountPerHeap;
};
}
