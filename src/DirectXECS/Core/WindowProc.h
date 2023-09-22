#pragma once

#include "Util/WindowsWrapper.h"

namespace DirectXECS::Core {
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
}
