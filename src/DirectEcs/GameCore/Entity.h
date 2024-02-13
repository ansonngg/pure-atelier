#pragma once

#include "Scene.h"

namespace DirectEcs
{
class Component;

class Entity : public std::enable_shared_from_this<Entity>
{
public:
    [[nodiscard]] uint32_t GetId() const;
    template<typename ComponentType, typename... Args>
    ComponentType& AddComponent(Args&& ...args) const;
    template<typename ComponentType>
    void RemoveComponent() const;

private:
    std::weak_ptr<Scene> m_Scene;
    uint32_t m_Id;

    Entity(const std::shared_ptr<Scene>& scene, uint32_t id);

    friend Scene;
};

template<typename ComponentType, typename... Args>
ComponentType& Entity::AddComponent(Args&& ...args) const
{
    return m_Scene.lock()->CreateComponent<ComponentType>(shared_from_this(), std::forward<Args>(args)...);
}

template<typename ComponentType>
void Entity::RemoveComponent() const
{
    m_Scene.lock()->RemoveComponent<ComponentType>(shared_from_this());
}
}
