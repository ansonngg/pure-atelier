#include "Entity.h"

#include "Scene.h"

namespace DirectXECS::ECS {
Entity::Entity(const std::shared_ptr<Scene> &scene, uint32_t id) : scene_(scene), id_(id) {}

uint32_t Entity::GetId() const {
    return id_;
}

template<typename ComponentType, typename... Args> requires std::is_base_of<Component, ComponentType>::value
Component *Entity::CreateComponent(Args &&...args) const {
    return scene_.lock()->CreateComponent(shared_from_this(), std::forward<Args>(args)...);
}
}
