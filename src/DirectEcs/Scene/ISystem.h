#pragma once

namespace DirectEcs
{
class ISystem
{
public:
    virtual void Update(double deltaSecond) = 0;

protected:
    virtual ~ISystem() = 0;
};
}
