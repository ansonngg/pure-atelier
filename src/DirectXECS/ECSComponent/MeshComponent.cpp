#include "MeshComponent.h"

namespace DirectXECS::ECSComponent {
MeshComponent::MeshComponent(const std::shared_ptr<ECSCore::Entity> &entity, const std::shared_ptr<Object::Mesh> &mesh)
    : ECSCore::Component(entity)
    , mesh_(mesh) {}
}
