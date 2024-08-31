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
struct ResourceHeapAllocation;

class ResourceHeapPage
{
public:
    explicit ResourceHeapPage(std::size_t size);
    ~ResourceHeapPage();

    [[nodiscard]] bool HasSpace(std::size_t size, std::size_t alignment) const;
    ResourceHeapAllocation Allocate(std::size_t size, std::size_t alignment);
    void Clear();

private:
    std::size_t m_PageSize;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_D3D12Resource;
    void* m_BaseCPUPtr = nullptr;
    D3D12_GPU_VIRTUAL_ADDRESS m_BaseGPUPtr = D3D12_GPU_VIRTUAL_ADDRESS(0);
    std::size_t m_Offset = 0;
};
}
