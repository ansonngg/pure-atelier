#include "Core/Application.h"

namespace DirectXECS {
int Main(HINSTANCE hInstance) {
    std::wstring windowName = L"DirectX ECS";
    uint32_t clientWidth = 1280;
    uint32_t clientHeight = 720;
    bool useVsync = false;
    bool useWarp = false;

    int argc;
    wchar_t **argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

    for (size_t i = 0; i < argc; ++i) {
        if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0) {
            clientWidth = ::wcstol(argv[++i], nullptr, 10);
        } else if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0) {
            clientHeight = ::wcstol(argv[++i], nullptr, 10);
        } else if (::wcscmp(argv[i], L"-v") == 0 || ::wcscmp(argv[i], L"--vsync") == 0) {
            useVsync = true;
        } else if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0) {
            useWarp = true;
        }
    }

    ::LocalFree(argv);

    Core::Application &app = Core::Application::GetInstance();
    app.Init(hInstance, windowName, clientWidth, clientHeight, useVsync, useWarp);
    return app.Exec();
}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    return DirectXECS::Main(hInstance);
}
