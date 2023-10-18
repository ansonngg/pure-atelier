#include "TransformComponent.h"

namespace DirectXECS::ECSComponent {
TransformComponent::TransformComponent(
    const std::shared_ptr<ECSCore::Entity> &entity,
    Vector3 position,
    Vector3 rotation,
    Vector3 scale
)
    : ECSCore::Component(entity)
    , position_(position)
    , rotation_(rotation)
    , scale_(scale) {}
}
