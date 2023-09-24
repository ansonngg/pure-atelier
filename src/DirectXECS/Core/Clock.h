#pragma once

#include "PCH.h"

namespace DirectXECS::Core {
class Clock {
public:
    double GetDeltaSecond();
    double GetTotalSecond();

    void Init();
    void Tick();

private:
    std::chrono::high_resolution_clock::time_point initialTime_;
    std::chrono::high_resolution_clock::duration deltaTime_{};
    std::chrono::high_resolution_clock::duration totalTime_{};
};
}
