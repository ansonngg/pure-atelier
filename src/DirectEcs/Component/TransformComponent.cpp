#include "TransformComponent.h"

namespace DirectEcs
{
TransformComponent::TransformComponent(
    const std::shared_ptr<Entity>& entity,
    Vector3 position,
    Vector3 rotation,
    Vector3 scale
)
    : Component(entity)
    , position_(position)
    , rotation_(rotation)
    , scale_(scale)
{
}
}
