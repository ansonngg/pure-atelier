#include "TransformComponent.h"

namespace DirectEcs
{
TransformComponent::TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale)
    : position_(position)
    , rotation_(rotation)
    , scale_(scale)
{
}
}
