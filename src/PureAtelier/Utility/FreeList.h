#include <utility>

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
struct FreeBlock;

class FreeList
{
public:
    explicit FreeList(uint32_t size);

    std::pair<uint32_t, bool> Allocate(uint32_t size);
    void Deallocate(uint32_t offset, uint32_t size);

private:
    using OffsetMap = std::map<uint32_t, FreeBlock>;
    using SizeMap = std::multimap<uint32_t, OffsetMap::iterator>;

    OffsetMap m_OffsetMap;
    SizeMap m_SizeMap;

    void AddNewBlock(uint32_t offset, uint32_t size);

    friend FreeBlock;
};

struct FreeBlock
{
    FreeList::SizeMap::iterator SizeIterator;
};
}
