/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

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
