/**
 * Copyright (c) 2024, ansonngg
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
*/

#include "Application.h"

#include "Window.h"

#include "Graphics/GraphicsDevice.h"
#include "Utility/Exception.h"

namespace PureAtelier
{
using Microsoft::WRL::ComPtr;

Application& Application::GetInstance()
{
    static Application application;
    return application;
}

uint64_t Application::GetFrameCount() const
{
    return m_Window->GetFrameCount();
}

void Application::Initialize(
    HINSTANCE hInstance,
    uint32_t clientWidth,
    uint32_t clientHeight,
    std::wstring&& windowName
)
{
    ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

#ifdef _DEBUG
    ComPtr<ID3D12Debug> debugInterface;
    ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
    debugInterface->EnableDebugLayer();
#endif

    GraphicsDevice::GetInstance().Initialize();
    m_Window = std::make_unique<Window>(clientWidth, clientHeight, std::move(windowName));
    m_Window->Initialize(hInstance);
    m_IsInitialized = true;
}

void Application::Start()
{
    if (!m_IsInitialized)
    {
        m_Message.message = WM_QUIT;
        m_Message.wParam = EXIT_FAILURE;
        return;
    }

    m_Window->Create();
}

void Application::Update()
{
    if (::PeekMessage(&m_Message, nullptr, 0, 0, PM_REMOVE))
    {
        ::TranslateMessage(&m_Message);
        ::DispatchMessage(&m_Message);
    }
}

bool Application::ShouldClose() const
{
    return m_Message.message == WM_QUIT;
}

int Application::Close()
{
    if (m_IsInitialized)
    {
        m_Window->Destroy();
        m_IsInitialized = false;
    }

    return (int)m_Message.wParam;
}

void Application::Flush()
{

}
}
