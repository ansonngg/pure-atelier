#pragma once

namespace DirectXECS::ECSCore {
class Component;
class Scene;

class Entity : public std::enable_shared_from_this<Entity> {
public:
    explicit Entity(const std::shared_ptr<Scene> &scene, uint32_t id);
    [[nodiscard]] uint32_t GetId() const;
    template<typename ComponentType, typename... Args> requires std::is_base_of<Component, ComponentType>::value
    Component *CreateComponent(Args &&...args) const;

private:
    std::weak_ptr<Scene> scene_;
    uint32_t id_;
};
}
