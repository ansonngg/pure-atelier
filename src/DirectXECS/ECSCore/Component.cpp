#include "Component.h"

#include "Entity.h"

namespace DirectXECS::ECSCore {
Entity *Component::GetEntity() const {
    return entity_.lock().get();
}

Component::Component(const std::shared_ptr<Entity> &entity) : entity_(entity) {}
}
