#include "Component.h"

#include "Entity.h"

namespace DirectXECS::ECS {
Component::Component(const std::shared_ptr<Entity> &entity) : entity_(entity) {}

Entity *Component::GetEntity() const {
    return entity_.lock().get();
}
}
