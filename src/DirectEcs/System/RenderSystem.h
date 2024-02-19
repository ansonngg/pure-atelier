#pragma once

#include "GameCore/ISystem.h"

namespace DirectEcs
{
class RenderSystem : public ISystem
{
public:
    void Update(double deltaSecond) override;
};
}
