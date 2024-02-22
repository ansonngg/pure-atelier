#pragma once

#include "Util/Concept.h"

namespace DirectEcs
{
class Entity;
class IComponent;

class IComponentContainer
{
public:
    virtual ~IComponentContainer() = default;
};

template<Derived<IComponent> ComponentType>
class ComponentContainer : public IComponentContainer
{
public:
    std::pair<ComponentType, std::weak_ptr<Entity>>& Back();
    ComponentType& PushBack(ComponentType&& component, std::shared_ptr<Entity>&& entity);
    void PopBack();
    std::size_t Size();
    std::pair<ComponentType, std::weak_ptr<Entity>>& operator[](std::size_t index);
    std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>>::iterator begin();
    std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>>::iterator end();

private:
    std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>> m_Components;
};

template<Derived<IComponent> ComponentType>
std::pair<ComponentType, std::weak_ptr<Entity>>& ComponentContainer<ComponentType>::operator[](std::size_t index)
{
    return m_Components[index];
}

template<Derived<IComponent> ComponentType>
std::pair<ComponentType, std::weak_ptr<Entity>>& ComponentContainer<ComponentType>::Back()
{
    return m_Components.back();
}

template<Derived<IComponent> ComponentType>
ComponentType& ComponentContainer<ComponentType>::PushBack(
    ComponentType&& component,
    std::shared_ptr<Entity>&& entity
)
{
    return m_Components.emplace_back(
        std::forward<ComponentType>(component),
        std::forward<std::shared_ptr<Entity>>(entity)
    ).first;
}

template<Derived<IComponent> ComponentType>
void ComponentContainer<ComponentType>::PopBack()
{
    m_Components.pop_back();
}

template<Derived<IComponent> ComponentType>
std::size_t ComponentContainer<ComponentType>::Size()
{
    return m_Components.size();
}

template<Derived<IComponent> ComponentType>
std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>>::iterator ComponentContainer<ComponentType>::begin()
{
    return m_Components.begin();
}

template<Derived<IComponent> ComponentType>
std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>>::iterator ComponentContainer<ComponentType>::end()
{
    return m_Components.end();
}
}
