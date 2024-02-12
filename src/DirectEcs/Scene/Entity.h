#pragma once

#include "Scene.h"

namespace DirectEcs
{
class Component;

class Entity : public std::enable_shared_from_this<Entity>
{
public:
    explicit Entity(const std::shared_ptr<Scene>& scene, uint32_t id);
    [[nodiscard]] uint32_t GetId() const;
    template<typename ComponentType, typename... Args>
    ComponentType& CreateComponent(Args&& ...args) const;

private:
    std::weak_ptr<Scene> m_pScene;
    uint32_t m_Id;
};

template<typename ComponentType, typename... Args>
ComponentType& Entity::CreateComponent(Args&& ...args) const
{
    return m_pScene.lock()->CreateComponent(shared_from_this(), std::forward<Args>(args)...);
}
}
