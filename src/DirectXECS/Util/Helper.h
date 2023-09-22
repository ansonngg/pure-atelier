#pragma once

#include <winerror.h>

namespace DirectXECS::Util {
void ThrowIfFailed(HRESULT hr);
}
