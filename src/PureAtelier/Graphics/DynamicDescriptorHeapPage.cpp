/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DynamicDescriptorHeapPage.h"

#include "DynamicDescriptorHeapAllocation.h"
#include "GraphicsDevice.h"

#include "Core/Application.h"
#include "Utility/FreeList.h"

namespace PureAtelier
{
StaleDescriptor::StaleDescriptor(uint32_t offset, uint32_t descriptorCount, uint64_t frameNum)
    : Offset(offset)
    , DescriptorCount(descriptorCount)
    , FrameNum(frameNum)
{
}

DynamicDescriptorHeapPage::DynamicDescriptorHeapPage(D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t descriptorCount)
    : m_HeapType(heapType)
    , m_DescriptorCountInHeap(descriptorCount)
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {
        .Type = m_HeapType,
        .NumDescriptors = m_DescriptorCountInHeap
    };

    auto& device = GraphicsDevice::GetInstance();
    device.CreateDescriptorHeap(heapDesc, m_D3D12DescriptorHeap);
    m_BaseDescriptor = m_D3D12DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    m_DescriptorHandleIncrementSize = device.GetDescriptorHandleIncrementSize(m_HeapType);
    m_FreeHandleCount = m_DescriptorCountInHeap;
    m_FreeList = std::make_unique<FreeList>(m_DescriptorCountInHeap);
}

uint32_t DynamicDescriptorHeapPage::GetFreeHandleCount() const
{
    return m_FreeHandleCount;
}

DynamicDescriptorHeapAllocation DynamicDescriptorHeapPage::Allocate(uint32_t descriptorCount)
{
    auto [offset, isSuccess] = m_FreeList->Allocate(descriptorCount);

    if (!isSuccess)
    {
        return {};
    }

    m_FreeHandleCount -= descriptorCount;

    return {
        CD3DX12_CPU_DESCRIPTOR_HANDLE(m_BaseDescriptor, (INT)offset, m_DescriptorHandleIncrementSize),
        descriptorCount,
        m_DescriptorHandleIncrementSize,
        shared_from_this()
    };
}

void DynamicDescriptorHeapPage::Free(DynamicDescriptorHeapAllocation&& allocation)
{
    auto offsetInSize = allocation.GetDescriptorHandle(0).ptr - m_BaseDescriptor.ptr;
    auto offset = (uint32_t)offsetInSize / m_DescriptorHandleIncrementSize;
    m_StaleDescriptorQueue.emplace(offset, allocation.GetHandleCount(), Application::GetInstance().GetFrameCount());
}

void DynamicDescriptorHeapPage::ReleaseStaleDescriptors()
{
    auto currentFrameNum = Application::GetInstance().GetFrameCount();

    while (!m_StaleDescriptorQueue.empty() && m_StaleDescriptorQueue.front().FrameNum <= currentFrameNum)
    {
        auto& staleDescriptor = m_StaleDescriptorQueue.front();
        m_StaleDescriptorQueue.pop();
        m_FreeList->Deallocate(staleDescriptor.Offset, staleDescriptor.DescriptorCount);
    }
}
}
