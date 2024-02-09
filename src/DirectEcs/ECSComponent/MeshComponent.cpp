#include "MeshComponent.h"

namespace DirectEcs
{
MeshComponent::MeshComponent(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Mesh>& mesh)
    : Component(entity)
    , mesh_(mesh)
{
}
}
