#include "Math.h"

namespace PureAtelier
{
std::size_t AlignUp(std::size_t value, std::size_t alignment)
{
    --alignment;
    return (value + alignment) & ~alignment;
}
}
