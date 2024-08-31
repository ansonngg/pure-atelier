/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Window.h"

#include "Application.h"

#include "Graphics/SwapChain.h"

#include <utility>

namespace PureAtelier
{
Window::Window(uint32_t clientWidth, uint32_t clientHeight, std::wstring&& windowName)
    : m_ClientWidth(clientWidth)
    , m_ClientHeight(clientHeight)
    , m_WindowName(std::move(windowName))
{
}

uint64_t Window::GetFrameCount() const
{
    return m_FrameCount;
}

void Window::Initialize(HINSTANCE hInstance)
{
    WNDCLASSEXW windowClass = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = &Process,
        .hInstance = hInstance,
        .hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(5)),
        .hCursor = ::LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszMenuName = nullptr,
        .lpszClassName = L"DX12WindowClass",
        .hIconSm = ::LoadIcon(hInstance, MAKEINTRESOURCE(5)),
    };

    if (!::RegisterClassExW(&windowClass))
    {
        ::MessageBoxA(nullptr, "Unable to register the window class.", "Error", MB_OK | MB_ICONERROR);
    }

    m_WindowRect = { 0, 0, (LONG)m_ClientWidth, (LONG)m_ClientHeight };
    ::AdjustWindowRect(&m_WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_WindowHandle = CreateWindowW(
        windowClass.lpszClassName,
        m_WindowName.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_WindowRect.right - m_WindowRect.left,
        m_WindowRect.bottom - m_WindowRect.top,
        nullptr,
        nullptr,
        hInstance,
        this
    );

    if (!m_WindowHandle)
    {
        ::MessageBoxA(nullptr, "Could not create the render window.", "Error", MB_OK | MB_ICONERROR);
    }

    m_SwapChain = std::make_unique<SwapChain>();
}

void Window::Create()
{
    ::ShowWindow(m_WindowHandle, SW_SHOW);
}

void Window::Destroy()
{
    ::DestroyWindow(m_WindowHandle);
    m_WindowHandle = nullptr;
}

void Window::Render()
{

}

void Window::ToggleFullscreen()
{
    m_IsFullscreen = !m_IsFullscreen;

    if (m_IsFullscreen)
    {
        // Store the current window dimensions so that they can be restored
        ::GetWindowRect(m_WindowHandle, &m_WindowRect);

        // Set the window style to borderless windowed
        ::SetWindowLongW(
            m_WindowHandle,
            GWL_STYLE,
            WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
        );

        // Query the name of the nearest display device for the window
        auto hMonitor = ::MonitorFromWindow(m_WindowHandle, MONITOR_DEFAULTTONEAREST);
        MONITORINFOEX monitorInfo = {
            {
                .cbSize = sizeof(MONITORINFOEX),
            },
        };
        ::GetMonitorInfo(hMonitor, &monitorInfo);

        ::SetWindowPos(
            m_WindowHandle,
            HWND_TOPMOST,
            monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.top,
            monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE
        );

        ::ShowWindow(m_WindowHandle, SW_MAXIMIZE);
    }
    else
    {
        // Restore all the window decorators
        ::SetWindowLong(m_WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);

        ::SetWindowPos(
            m_WindowHandle,
            HWND_NOTOPMOST,
            m_WindowRect.left,
            m_WindowRect.top,
            m_WindowRect.right - m_WindowRect.left,
            m_WindowRect.bottom - m_WindowRect.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE
        );

        ::ShowWindow(m_WindowHandle, SW_NORMAL);
    }
}

void Window::Resize(uint32_t width, uint32_t height)
{
    if (width == m_ClientWidth && height == m_ClientHeight)
    {
        return;
    }

    m_ClientWidth = std::max(1u, width);
    m_ClientHeight = std::max(1u, height);
    Application::GetInstance().Flush();
    m_SwapChain->ResizeBuffers(m_ClientWidth, m_ClientHeight);
}

LRESULT Window::Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto window = (Window*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {
    case WM_PAINT:
        ++window->m_FrameCount;
        window->Render();
        break;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    {
        bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

        switch (wParam)
        {
        case VK_ESCAPE:
            ::PostQuitMessage(0);
            break;
        case VK_RETURN:
            if (alt)
            {
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
    case WM_SIZE:
    {
        RECT clientRect{};
        ::GetClientRect(hWnd, &clientRect);

        uint32_t width = clientRect.right - clientRect.left;
        uint32_t height = clientRect.bottom - clientRect.top;
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
