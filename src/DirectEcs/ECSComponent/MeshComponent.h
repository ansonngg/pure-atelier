#pragma once

#include "ECSCore/Component.h"

namespace DirectEcs
{
class Mesh;

class MeshComponent : public Component
{
public:
    MeshComponent(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Mesh>& mesh);

private:
    std::shared_ptr<Mesh> mesh_;
};
}
