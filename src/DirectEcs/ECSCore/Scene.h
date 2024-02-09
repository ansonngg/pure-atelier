#pragma once

namespace DirectEcs
{
class ComponentContainerBase;
class Component;
class Entity;
class System;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
    void Update(double deltaSecond, double totalSecond);
    Entity* CreateEntity();
    template<typename ComponentType, typename... Args>
    requires std::is_base_of<Component, ComponentType>::value
    ComponentType* CreateComponent(std::shared_ptr<Entity> entity, Args&& ...args);

private:
    std::unordered_set<std::unique_ptr<Entity>> entitySet_;
    std::unordered_map<size_t, std::unique_ptr<ComponentContainerBase>> componentContainerMap_;
    std::vector<std::unique_ptr<System>> systems_;
    uint32_t nextEntityId_ = 0;
};
}
