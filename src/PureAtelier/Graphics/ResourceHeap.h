#pragma once

namespace PureAtelier
{
struct ResourceHeapAllocation;
class ResourceHeapPage;

class ResourceHeap
{
public:
    // 2097152 B = 2 MB
    explicit ResourceHeap(std::size_t pageSize = 2097152);

    ResourceHeapAllocation Allocate(std::size_t size, std::size_t alignment);
    void Clear();
private:
    std::size_t m_PageSize;
    std::vector<std::unique_ptr<ResourceHeapPage>> m_PagePool;
    std::size_t m_CurrentPageIndex = 0;

    void RequestPage();
};
}
