#include "Application.h"

// System headers
#include <cassert>

// Local headers
#include "Util/Helper.h"
#include "WindowProc.h"

using Microsoft::WRL::ComPtr;

namespace DirectXECS::Core {
Application &Application::GetInstance() {
    static Application application;
    return application;
}

Window *Application::GetWindow() {
    return window_.get();
}

ComPtr<ID3D12Device2> Application::GetD3D12Device() {
    return d3d12Device_;
}

CommandQueue *Application::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type) {
    switch (type) {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
        return directCommandQueue_.get();
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
        return computeCommandQueue_.get();
    case D3D12_COMMAND_LIST_TYPE_COPY:
        return copyCommandQueue_.get();
    default:
        assert(false && "Invalid command queue type.");
        return nullptr;
    }
}

void Application::Init(
    HINSTANCE hInstance,
    const std::wstring &windowName,
    uint32_t clientWidth,
    uint32_t clientHeight,
    bool useVsync,
    bool useWarp
) {
    ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

#if defined(_DEBUG)
    ComPtr<ID3D12Debug> debugInterface;
    Util::ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
    debugInterface->EnableDebugLayer();
#endif

    CreateDevice_(useWarp);
    CreateCommandQueues_();
    CreateWindow_(hInstance, windowName, clientWidth, clientHeight, useVsync);
    isInitialized_ = true;
}

int Application::Exec() {
    if (!isInitialized_) {
        return EXIT_FAILURE;
    }

    window_->Start();
    MSG msg{};

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Flush();
    window_->Destroy();
    return (int)msg.wParam;
}

void Application::Flush() {
    directCommandQueue_->Flush();
    computeCommandQueue_->Flush();
    copyCommandQueue_->Flush();
}

void Application::CreateWindow_(
    HINSTANCE hInstance,
    const std::wstring &windowName,
    uint32_t clientWidth,
    uint32_t clientHeight,
    bool useVsync
) {
    WNDCLASSEXW windowClass = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = &WindowProc,
        .hInstance = hInstance,
        .hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(5)),
        .hCursor = ::LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszMenuName = nullptr,
        .lpszClassName = L"DX12WindowClass",
        .hIconSm = ::LoadIcon(hInstance, MAKEINTRESOURCE(5)),
    };

    if (!::RegisterClassExW(&windowClass)) {
        MessageBoxA(nullptr, "Unable to register the window class.", "Error", MB_OK | MB_ICONERROR);
    }

    RECT windowRect = { 0, 0, (LONG)clientWidth, (LONG)clientHeight };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND windowHandle = CreateWindowW(
        windowClass.lpszClassName,
        windowName.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!windowHandle) {
        MessageBoxA(nullptr, "Could not create the render window.", "Error", MB_OK | MB_ICONERROR);
    }

    window_ = std::make_unique<Window>(windowHandle, clientWidth, clientHeight, useVsync);
}

void Application::CreateDevice_(bool useWarp) {
    ComPtr<IDXGIAdapter4> dxgiAdapter = GetAdapter_(useWarp);
    Util::ThrowIfFailed(D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device_)));

    // Enable debug messages in debug mode.
#ifdef _DEBUG
    ComPtr<ID3D12InfoQueue> infoQueue;

    if (SUCCEEDED(d3d12Device_.As(&infoQueue))) {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

        // Suppress messages based on their severity level
        D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };

        // Suppress individual messages by their ID
        D3D12_MESSAGE_ID denyIds[] = {
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
        };

        D3D12_INFO_QUEUE_FILTER newFilter = {
            .DenyList = {
                .NumSeverities = _countof(severities),
                .pSeverityList = severities,
                .NumIDs = _countof(denyIds),
                .pIDList = denyIds,
            },
        };

        Util::ThrowIfFailed(infoQueue->PushStorageFilter(&newFilter));
    }
#endif // _DEBUG
}

void Application::CreateCommandQueues_() {
    directCommandQueue_ = std::make_unique<CommandQueue>(D3D12_COMMAND_LIST_TYPE_DIRECT);
    computeCommandQueue_ = std::make_unique<CommandQueue>(D3D12_COMMAND_LIST_TYPE_COMPUTE);
    copyCommandQueue_ = std::make_unique<CommandQueue>(D3D12_COMMAND_LIST_TYPE_COPY);
}

ComPtr<IDXGIAdapter4> Application::GetAdapter_(bool useWarp) {
    ComPtr<IDXGIFactory4> dxgiFactory;
    UINT createFactoryFlags;

#ifdef _DEBUG
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#else
    createFactoryFlags = 0;
#endif // _DEBUG

    Util::ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

    ComPtr<IDXGIAdapter1> dxgiAdapter1;
    ComPtr<IDXGIAdapter4> dxgiAdapter4;

    if (useWarp) {
        Util::ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
        Util::ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
    } else {
        SIZE_T maxDedicatedVideoMemory = 0;

        for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i) {
            DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
            dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

            // Pick the adapter with the largest dedicated video memory
            if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0
                && SUCCEEDED(
                    D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)
                )
                && dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory) {
                maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
                Util::ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
            }
        }
    }

    return dxgiAdapter4;
}
}
