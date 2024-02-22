#pragma once

#include "GameCore/IComponent.h"

namespace DirectEcs
{
class Mesh;

class MeshComponent : public IComponent
{
public:
    explicit MeshComponent(const std::shared_ptr<Mesh>& mesh);

private:
    std::shared_ptr<Mesh> m_Mesh;
};
}
