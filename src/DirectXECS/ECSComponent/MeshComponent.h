#pragma once

#include "ECSCore/Component.h"

namespace DirectXECS {
namespace Object {
class Mesh;
}

namespace ECSComponent {
class MeshComponent : public ECSCore::Component {
public:
    MeshComponent(const std::shared_ptr<ECSCore::Entity> &entity, const std::shared_ptr<Object::Mesh> &mesh);

private:
    std::shared_ptr<Object::Mesh> mesh_;
};
}
}
