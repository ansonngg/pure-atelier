#pragma once

namespace PureAtelier
{
class Window;

class Application
{
public:
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    static Application& GetInstance();

    [[nodiscard]] uint64_t GetFrameCount() const;

    void Initialize(HINSTANCE hInstance, uint32_t clientWidth, uint32_t clientHeight, std::wstring&& windowName);
    void Start();
    void Update();
    [[nodiscard]] bool ShouldClose() const;
    [[nodiscard]] int Close();
    void Flush();

private:
    std::unique_ptr<Window> m_Window;
    MSG m_Message = {};
    bool m_IsInitialized = false;

    Application() = default;
    ~Application() = default;
};
}
