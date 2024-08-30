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

class DynamicDescriptorHeapPage
{
public:
    DynamicDescriptorHeapPage(D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t descriptorCount);

    void Free(DynamicDescriptorHeapAllocation&& allocation);

private:
    D3D12_DESCRIPTOR_HEAP_TYPE m_HeapType;
    uint32_t m_DescriptorCount;
};
}
