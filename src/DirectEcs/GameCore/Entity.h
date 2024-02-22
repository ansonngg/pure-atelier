#pragma once

#include "Scene.h"

#include "Util/Concept.h"

namespace DirectEcs
{
class IComponent;

class Entity : public std::enable_shared_from_this<Entity>
{
public:
    [[nodiscard]] uint32_t GetId() const;
    template<Derived<IComponent> ComponentType, typename... Args>
    ComponentType& AddComponent(Args&& ...args);
    template<Derived<IComponent> ComponentType>
    void RemoveComponent();

private:
    std::weak_ptr<Scene> m_Scene;
    uint32_t m_Id;

    Entity(const std::shared_ptr<Scene>& scene, uint32_t id);

    friend Scene;
};

template<Derived<IComponent> ComponentType, typename... Args>
ComponentType& Entity::AddComponent(Args&& ...args)
{
    return m_Scene.lock()->CreateComponent<ComponentType>(shared_from_this(), std::forward<Args>(args)...);
}

template<Derived<IComponent> ComponentType>
void Entity::RemoveComponent()
{
    m_Scene.lock()->RemoveComponent<ComponentType>(shared_from_this());
}
}
