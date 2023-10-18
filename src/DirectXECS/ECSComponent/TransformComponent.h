#pragma once

#include "ECSCore/Component.h"

namespace DirectXECS::ECSComponent {
class TransformComponent : public ECSCore::Component {
public:
    TransformComponent(
        const std::shared_ptr<ECSCore::Entity> &entity,
        Vector3 position,
        Vector3 rotation,
        Vector3 scale
    );

private:
    Vector3 position_;
    Vector3 rotation_;
    Vector3 scale_;
};
}
