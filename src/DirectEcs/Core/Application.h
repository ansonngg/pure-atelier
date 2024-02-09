#pragma once

namespace DirectEcs
{
class CommandQueue;
class Window;

class Application
{
public:
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    static Application& GetInstance();

    Window* GetWindow();
    Microsoft::WRL::ComPtr<ID3D12Device2> GetD3D12Device();
    CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type);

    void Init(
        HINSTANCE hInstance,
        const std::wstring& windowName,
        uint32_t clientWidth,
        uint32_t clientHeight,
        bool useVsync,
        bool useWarp
    );
    int Exec();
    void Flush();

private:
    std::unique_ptr<Window> window_;
    Microsoft::WRL::ComPtr<ID3D12Device2> d3d12Device_;
    std::unique_ptr<CommandQueue> directCommandQueue_;
    std::unique_ptr<CommandQueue> computeCommandQueue_;
    std::unique_ptr<CommandQueue> copyCommandQueue_;
    bool isInitialized_ = false;

    Application() = default;
    ~Application() = default;

    void CreateWindow_(
        HINSTANCE hInstance,
        const std::wstring& windowName,
        uint32_t clientWidth,
        uint32_t clientHeight,
        bool useVsync
    );
    void CreateDevice_(bool useWarp);
    void CreateCommandQueues_();

    static Microsoft::WRL::ComPtr<IDXGIAdapter4> GetAdapter_(bool useWarp);
};
}
