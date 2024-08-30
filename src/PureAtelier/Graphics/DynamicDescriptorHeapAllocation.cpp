#include "DynamicDescriptorHeapAllocation.h"

#include "DynamicDescriptorHeapPage.h"

namespace PureAtelier
{
DynamicDescriptorHeapAllocation::DynamicDescriptorHeapAllocation()
    : m_DescriptorHandle{}
    , m_HandleCount(0)
    , m_DescriptorSize(0)
{
}

DynamicDescriptorHeapAllocation::DynamicDescriptorHeapAllocation(
    D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle,
    uint32_t handleCount,
    uint32_t descriptorSize,
    const std::shared_ptr<DynamicDescriptorHeapPage>& page
)
    : m_DescriptorHandle(descriptorHandle)
    , m_HandleCount(handleCount)
    , m_DescriptorSize(descriptorSize)
    , m_Page(page)
{
}

DynamicDescriptorHeapAllocation::DynamicDescriptorHeapAllocation(DynamicDescriptorHeapAllocation&& allocation) noexcept
    : m_DescriptorHandle(allocation.m_DescriptorHandle)
    , m_HandleCount(allocation.m_HandleCount)
    , m_DescriptorSize(allocation.m_DescriptorSize)
    , m_Page(allocation.m_Page)
{
    allocation.m_DescriptorHandle.ptr = 0;
    allocation.m_HandleCount = 0;
    allocation.m_DescriptorSize = 0;
}

DynamicDescriptorHeapAllocation&
DynamicDescriptorHeapAllocation::operator=(DynamicDescriptorHeapAllocation&& other) noexcept
{
    Free();

    m_DescriptorHandle = other.m_DescriptorHandle;
    m_HandleCount = other.m_HandleCount;
    m_DescriptorSize = other.m_DescriptorSize;
    m_Page = std::move(other.m_Page);

    other.m_DescriptorHandle.ptr = 0;
    other.m_HandleCount = 0;
    other.m_DescriptorSize = 0;

    return *this;
}

DynamicDescriptorHeapAllocation::~DynamicDescriptorHeapAllocation()
{
    Free();
}

D3D12_CPU_DESCRIPTOR_HANDLE DynamicDescriptorHeapAllocation::GetDescriptorHandle(uint32_t offset) const
{
    assert(offset < m_HandleCount);
    return { m_DescriptorHandle.ptr + m_DescriptorSize * offset };
}

uint32_t DynamicDescriptorHeapAllocation::GetHandleCount() const
{
    return m_HandleCount;
}

bool DynamicDescriptorHeapAllocation::IsNull() const
{
    return m_DescriptorHandle.ptr == 0;
}

void DynamicDescriptorHeapAllocation::Free()
{
    if (!IsNull() && m_Page)
    {
        m_Page->Free(std::move(*this));
        m_DescriptorHandle.ptr = 0;
        m_HandleCount = 0;
        m_DescriptorSize = 0;
        m_Page.reset();
    }
}
}
