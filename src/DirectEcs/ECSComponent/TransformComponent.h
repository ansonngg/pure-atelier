#pragma once

#include "ECSCore/Component.h"

namespace DirectEcs
{
class TransformComponent : public Component
{
public:
    TransformComponent(
        const std::shared_ptr<Entity>& entity,
        Vector3 position,
        Vector3 rotation,
        Vector3 scale
    );

private:
    Vector3 position_;
    Vector3 rotation_;
    Vector3 scale_;
};
}
