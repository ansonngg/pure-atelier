#include "MeshComponent.h"

namespace DirectEcs
{
MeshComponent::MeshComponent(const std::shared_ptr<Mesh>& mesh) : m_Mesh(mesh)
{
}
}
