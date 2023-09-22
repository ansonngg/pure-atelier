#include "Clock.h"

using std::chrono::high_resolution_clock;

namespace DirectXECS::Core {
double Clock::GetDeltaSecond() {
    return (double)deltaTime_.count() * 1e-9;
}

double Clock::GetTotalSecond() {
    return (double)totalTime_.count() * 1e-9;
}

void Clock::Init() {
    initialTime_ = high_resolution_clock::now();
    deltaTime_ = high_resolution_clock::duration();
    totalTime_ = high_resolution_clock::duration();
}

void Clock::Tick() {
    high_resolution_clock::duration oldTotalTime = totalTime_;
    totalTime_ = high_resolution_clock::now() - initialTime_;
    deltaTime_ = totalTime_ - oldTotalTime;
}
}
