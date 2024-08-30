/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
*/

#include "ResourceHeap.h"

#include "ResourceHeapAllocation.h"
#include "ResourceHeapPage.h"

namespace PureAtelier
{
ResourceHeap::ResourceHeap(std::size_t pageSize) : m_PageSize(pageSize)
{
}

ResourceHeapAllocation ResourceHeap::Allocate(std::size_t size, std::size_t alignment)
{
    if (size > m_PageSize)
    {
        throw std::bad_alloc();
    }

    if (m_PagePool.empty() || !m_PagePool[m_CurrentPageIndex]->HasSpace(size, alignment))
    {
        RequestPage();
    }

    return m_PagePool[m_CurrentPageIndex]->Allocate(size, alignment);
}

void ResourceHeap::Clear()
{
    m_CurrentPageIndex = 0;

    for (auto& page : m_PagePool)
    {
        page->Clear();
    }
}

void ResourceHeap::RequestPage()
{
    if (++m_CurrentPageIndex >= m_PagePool.size())
    {
        m_PagePool.emplace_back(std::make_unique<ResourceHeapPage>(m_PageSize));
    }
}
}
