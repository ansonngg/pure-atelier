/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ResourceHeapPage.h"

#include "GraphicsDevice.h"
#include "ResourceHeapAllocation.h"

#include "Utility/Math.h"

namespace PureAtelier
{
ResourceHeapPage::ResourceHeapPage(std::size_t pageSize) : m_PageSize(pageSize)
{
    GraphicsDevice::GetInstance().CreateCommittedResource(pageSize, m_D3D12Resource);
    m_D3D12Resource->Map(0, nullptr, &m_BaseCPUPtr);
    m_BaseGPUPtr = m_D3D12Resource->GetGPUVirtualAddress();
}

ResourceHeapPage::~ResourceHeapPage()
{
    m_D3D12Resource->Unmap(0, nullptr);
    m_BaseCPUPtr = nullptr;
    m_BaseGPUPtr = D3D12_GPU_VIRTUAL_ADDRESS(0);
}

bool ResourceHeapPage::HasSpace(std::size_t size, std::size_t alignment) const
{
    return AlignUp(m_Offset, alignment) + AlignUp(size, alignment) <= m_PageSize;
}

ResourceHeapAllocation ResourceHeapPage::Allocate(std::size_t size, std::size_t alignment)
{
    m_Offset = AlignUp(m_Offset, alignment);
    ResourceHeapAllocation allocation = {
        .CPUPtr = static_cast<uint8_t*>(m_BaseCPUPtr) + m_Offset,
        .GPUPtr = m_BaseGPUPtr + m_Offset
    };
    m_Offset += AlignUp(size, alignment);
    return allocation;
}

void ResourceHeapPage::Clear()
{
    m_Offset = 0;
}
}
