#include "TransformComponent.h"

namespace DirectEcs
{
TransformComponent::TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale)
    : m_Position(position)
    , m_Rotation(rotation)
    , m_Scale(scale)
{
}
}
