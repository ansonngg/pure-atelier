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
struct ResourceHeapAllocation
{
    void* CPUPtr;
    D3D12_GPU_VIRTUAL_ADDRESS GPUPtr;
};
}
