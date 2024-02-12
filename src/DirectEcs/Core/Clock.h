#pragma once

namespace DirectEcs
{
class Clock
{
public:
    double GetDeltaSecond();
    [[maybe_unused]] double GetTotalSecond();

    void Init();
    void Tick();

private:
    std::chrono::high_resolution_clock::time_point m_InitialTime;
    std::chrono::high_resolution_clock::duration m_DeltaTime{};
    std::chrono::high_resolution_clock::duration m_TotalTime{};
};
}
