#pragma once

#include "GameCore/IComponent.h"

namespace DirectEcs
{
class TransformComponent : public IComponent
{
public:
    TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale);

private:
    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Scale;
};
}
