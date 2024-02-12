#include "Clock.h"

using std::chrono::high_resolution_clock;

namespace DirectEcs
{
double Clock::GetDeltaSecond()
{
    return (double)m_DeltaTime.count() * 1e-9;
}

[[maybe_unused]] double Clock::GetTotalSecond()
{
    return (double)m_TotalTime.count() * 1e-9;
}

void Clock::Init()
{
    m_InitialTime = high_resolution_clock::now();
    m_DeltaTime = high_resolution_clock::duration();
    m_TotalTime = high_resolution_clock::duration();
}

void Clock::Tick()
{
    high_resolution_clock::duration oldTotalTime = m_TotalTime;
    m_TotalTime = high_resolution_clock::now() - m_InitialTime;
    m_DeltaTime = m_TotalTime - oldTotalTime;
}
}
