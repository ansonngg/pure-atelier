#pragma once

namespace DirectEcs
{
class System
{
public:
    virtual void Update(double deltaSecond, double totalSecond) = 0;
};
}
