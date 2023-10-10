#pragma once

namespace DirectXECS::ECS {
class System {
public:
    virtual void Update(double deltaSecond, double totalSecond) = 0;
};
}
