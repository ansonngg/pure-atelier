#pragma once

namespace PureAtelier
{
class DynamicDescriptorHeapPage;

class DynamicDescriptorHeapAllocation
{
public:
    DynamicDescriptorHeapAllocation();
    DynamicDescriptorHeapAllocation(
        D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle,
        uint32_t handleCount,
        uint32_t descriptorSize,
        const std::shared_ptr<DynamicDescriptorHeapPage>& page
    );
    DynamicDescriptorHeapAllocation(const DynamicDescriptorHeapAllocation&) = delete;
    DynamicDescriptorHeapAllocation& operator=(const DynamicDescriptorHeapAllocation&) = delete;
    DynamicDescriptorHeapAllocation(DynamicDescriptorHeapAllocation&& allocation) noexcept;
    DynamicDescriptorHeapAllocation& operator=(DynamicDescriptorHeapAllocation&& other) noexcept;
    ~DynamicDescriptorHeapAllocation();

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle(uint32_t offset) const;
    [[nodiscard]] uint32_t GetHandleCount() const;
    [[nodiscard]] bool IsNull() const;

private:
    D3D12_CPU_DESCRIPTOR_HANDLE m_DescriptorHandle;
    uint32_t m_HandleCount;
    uint32_t m_DescriptorSize;
    std::shared_ptr<DynamicDescriptorHeapPage> m_Page;

    void Free();
};
}
