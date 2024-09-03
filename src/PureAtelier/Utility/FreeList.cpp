/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FreeList.h"

namespace PureAtelier
{
FreeList::FreeList(uint32_t size)
{
    AddNewBlock(0, size);
}

std::pair<uint32_t, bool> FreeList::Allocate(uint32_t size)
{
    auto availableBlockIterator = m_SizeMap.lower_bound(size);

    if (availableBlockIterator == m_SizeMap.end())
    {
        return { 0, false };
    }

    auto [blockSize, offsetIterator] = *availableBlockIterator;
    auto offset = offsetIterator->first;
    m_OffsetMap.erase(offsetIterator);
    m_SizeMap.erase(availableBlockIterator);

    auto newSize = blockSize - size;

    if (newSize > 0)
    {
        AddNewBlock(offset + size, newSize);
    }

    return { offset, true };
}

void FreeList::Deallocate(uint32_t offset, uint32_t size)
{
    auto adjacentBlockIterator = m_OffsetMap.upper_bound(offset);

    if (adjacentBlockIterator != m_OffsetMap.end() && adjacentBlockIterator->first == offset + size)
    {
        auto sizeIterator = adjacentBlockIterator->second.SizeIterator;
        size += sizeIterator->first;
        adjacentBlockIterator = m_OffsetMap.erase(adjacentBlockIterator);
        m_SizeMap.erase(sizeIterator);
    }

    if (adjacentBlockIterator != m_OffsetMap.begin())
    {
        --adjacentBlockIterator;
        auto sizeIterator = adjacentBlockIterator->second.SizeIterator;

        if (adjacentBlockIterator->first + sizeIterator->first == offset)
        {
            size += sizeIterator->first;
            m_SizeMap.erase(sizeIterator);
            sizeIterator = m_SizeMap.emplace(size, adjacentBlockIterator);
            adjacentBlockIterator->second.SizeIterator = sizeIterator;
            return;
        }
    }

    AddNewBlock(offset, size);
}

void FreeList::AddNewBlock(uint32_t offset, uint32_t size)
{
    auto offsetIterator = m_OffsetMap.emplace(offset, FreeBlock{}).first;
    auto sizeIterator = m_SizeMap.emplace(size, offsetIterator);
    offsetIterator->second.SizeIterator = sizeIterator;
}
}
