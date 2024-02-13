#pragma once

namespace DirectEcs
{
class Entity;

class IComponentContainer
{
protected:
    virtual ~IComponentContainer() = 0;
};

template<typename ComponentType>
class ComponentContainer : public IComponentContainer
{
public:
    std::pair<ComponentType, std::weak_ptr<Entity>>& Back() const;
    ComponentType* PushBack(ComponentType&& component, std::shared_ptr<Entity>&& entity);
    void PopBack();
    size_t Size();
    std::pair<ComponentType, std::weak_ptr<Entity>>& operator[](size_t index) const;
    std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>>::iterator begin() const;
    std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>>::iterator end() const;

private:
    std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>> m_Components;
};

template<typename ComponentType>
std::pair<ComponentType, std::weak_ptr<Entity>>& ComponentContainer<ComponentType>::operator[](size_t index) const
{
    return m_Components[index];
}

template<typename ComponentType>
std::pair<ComponentType, std::weak_ptr<Entity>>& ComponentContainer<ComponentType>::Back() const
{
    return m_Components.back();
}

template<typename ComponentType>
ComponentType* ComponentContainer<ComponentType>::PushBack(
    ComponentType&& component,
    std::shared_ptr<Entity>&& entity
)
{
    return &m_Components.emplace_back(
        std::forward<ComponentType>(component),
        std::forward<std::shared_ptr<Entity>>(entity)
    ).first;
}

template<typename ComponentType>
void ComponentContainer<ComponentType>::PopBack()
{
    m_Components.pop_back();
}

template<typename ComponentType>
size_t ComponentContainer<ComponentType>::Size()
{
    return m_Components.size();
}

template<typename ComponentType>
std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>>::iterator ComponentContainer<ComponentType>::begin() const
{
    return m_Components.begin();
}

template<typename ComponentType>
std::vector<std::pair<ComponentType, std::weak_ptr<Entity>>>::iterator ComponentContainer<ComponentType>::end() const
{
    return m_Components.end();
}
}
