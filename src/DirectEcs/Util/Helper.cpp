#include "Helper.h"

namespace DirectEcs
{
void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}
}
