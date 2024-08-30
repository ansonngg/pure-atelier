#pragma once

namespace PureAtelier
{
struct ResourceHeapAllocation
{
    void* CPUPtr;
    D3D12_GPU_VIRTUAL_ADDRESS GPUPtr;
};
}
