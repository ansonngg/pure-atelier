#include "Helper.h"

namespace DirectXECS::Util {
void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        throw std::exception();
    }
}
}
