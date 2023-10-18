#include "Core/Application.h"

namespace DirectXECS {
#ifdef _DEBUG
int Main(int argc, char **argv) {
#else
int Main(HINSTANCE hInstance) {
#endif // _DEBUG
    std::wstring windowName = L"DirectX ECSCore";
    uint32_t clientWidth = 1280;
    uint32_t clientHeight = 720;
    bool useVsync = false;
    bool useWarp = false;

#ifdef _DEBUG
    for (size_t i = 0; i < argc; ++i) {
        if (std::strcmp(argv[i], "-w") == 0 || std::strcmp(argv[i], "--width") == 0) {
            clientWidth = std::stoi(argv[++i], nullptr);
        } else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--height") == 0) {
            clientHeight = std::stoi(argv[++i], nullptr);
        } else if (std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--vsync") == 0) {
            useVsync = true;
        } else if (std::strcmp(argv[i], "-warp") == 0 || std::strcmp(argv[i], "--warp") == 0) {
            useWarp = true;
        }
    }

    HINSTANCE hInstance = ::GetModuleHandle(nullptr);
#else
    int argc;
    wchar_t **argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

    for (size_t i = 0; i < argc; ++i) {
        if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0) {
            clientWidth = std::wcstol(argv[++i], nullptr, 10);
        } else if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0) {
            clientHeight = std::wcstol(argv[++i], nullptr, 10);
        } else if (::wcscmp(argv[i], L"-v") == 0 || ::wcscmp(argv[i], L"--vsync") == 0) {
            useVsync = true;
        } else if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0) {
            useWarp = true;
        }
    }

    ::LocalFree(argv);
#endif // _DEBUG

    Core::Application &app = Core::Application::GetInstance();
    app.Init(hInstance, windowName, clientWidth, clientHeight, useVsync, useWarp);
    return app.Exec();
}
}

#ifdef _DEBUG
int main(int argc, char **argv) {
    return DirectXECS::Main(argc, argv);
}
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    return DirectXECS::Main(hInstance);
}
#endif // _DEBUG
