#include "WindowProc.h"

// Local headers
#include "Application.h"

namespace DirectXECS::Core {
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Window *window = Application::GetInstance().GetWindow();

    switch (message) {
    case WM_PAINT:
        window->Update();
        break;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN: {
        bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

        switch (wParam) {
        case VK_ESCAPE:
            ::PostQuitMessage(0);
            break;
        case VK_RETURN:
            if (alt) {
            case VK_F11:
                window->ToggleFullscreen();
            }
            break;
        default:
            break;
        }
    }
    case WM_SYSCHAR:
        break;
    case WM_SIZE: {
        RECT clientRect{};
        ::GetClientRect(hWnd, &clientRect);

        int width = clientRect.right - clientRect.left;
        int height = clientRect.bottom - clientRect.top;
        window->Resize(width, height);

        break;
    }
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProcW(hWnd, message, wParam, lParam);
    }

    return 0;
}
}
