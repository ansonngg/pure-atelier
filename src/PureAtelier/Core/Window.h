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
class SwapChain;

class Window
{
public:
    Window(uint32_t clientWidth, uint32_t clientHeight, std::wstring&& windowName);

    [[nodiscard]] uint64_t GetFrameCount() const;

    void Initialize(HINSTANCE hInstance);
    void Create();
    void Destroy();
    void Render();
    void ToggleFullscreen();
    void Resize(uint32_t width, uint32_t height);

    static LRESULT CALLBACK Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    uint32_t m_ClientWidth;
    uint32_t m_ClientHeight;
    std::wstring m_WindowName;
    HWND m_WindowHandle{};
    RECT m_WindowRect{};
    std::unique_ptr<SwapChain> m_SwapChain;
    uint64_t m_FrameCount = 0;
    bool m_IsFullscreen = false;
};
}
