#include "DynamicDescriptorHeap.h"

namespace PureAtelier
{
DynamicDescriptorHeap::DynamicDescriptorHeap(
    D3D12_DESCRIPTOR_HEAP_TYPE heapType,
    uint32_t unstagedDescriptorCountPerHeap,
    uint32_t stagedDescriptorCountPerHeap
)
    : m_HeapType(heapType)
    , m_UnstagedDescriptorCountPerHeap(unstagedDescriptorCountPerHeap)
    , m_StagedDescriptorCountPerHeap(stagedDescriptorCountPerHeap)
{
}
}
