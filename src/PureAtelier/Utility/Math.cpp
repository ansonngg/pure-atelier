/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
*/

#include "Math.h"

namespace PureAtelier
{
std::size_t AlignUp(std::size_t value, std::size_t alignment)
{
    --alignment;
    return (value + alignment) & ~alignment;
}
}
