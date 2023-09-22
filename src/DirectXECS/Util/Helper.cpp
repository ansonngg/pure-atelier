#include "Helper.h"

// System headers
#include <exception>

namespace DirectXECS::Util {
void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        throw std::exception();
    }
}
}
