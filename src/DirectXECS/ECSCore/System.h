#pragma once

namespace DirectXECS::ECSCore {
class System {
public:
    virtual void Update(double deltaSecond, double totalSecond) = 0;
};
}
