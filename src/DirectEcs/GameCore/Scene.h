#pragma once

#include "ComponentContainer.h"

#include "Util/Logger.h"

namespace DirectEcs
{
class Entity;
class ISystem;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
    void Update(double deltaSecond);
    std::shared_ptr<Entity> CreateEntity();

private:
    std::unordered_map<std::shared_ptr<Entity>, std::unordered_map<std::type_index, size_t>> m_EntityToComponentMap;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentContainer>> m_ComponentContainerMap;
    std::vector<std::unique_ptr<ISystem>> m_Systems;
    uint32_t m_NextEntityId = 0;

    template<typename ComponentType, typename... Args>
    ComponentType& CreateComponent(std::shared_ptr<Entity> entity, Args&& ...args);
    template<typename ComponentType>
    void RemoveComponent(const std::shared_ptr<Entity>& entity);
    template<typename ComponentType>
    ComponentContainer<ComponentType>& GetOrCreateComponentContainer() const;

    friend Entity;
};

template<typename ComponentType, typename... Args>
ComponentType& Scene::CreateComponent(std::shared_ptr<Entity> entity, Args&& ...args)
{
    auto componentTypeHash = std::type_index(typeid(ComponentType));
    std::unordered_map<std::type_index, size_t>& componentMap = m_EntityToComponentMap[entity];
    auto componentIterator = componentMap.find(componentTypeHash);
    ComponentContainer<ComponentType>& componentContainer = GetOrCreateComponentContainer<ComponentType>();

    if (componentIterator != componentMap.end())
    {
        Logger::Warn("The same component has already been created on the same entity");
        return componentContainer[componentIterator->second].first;
    }

    componentMap.emplace(componentTypeHash, componentContainer.Size());
    return componentContainer.PushBack(ComponentType(std::forward<Args>(args)...), entity);
}

template<typename ComponentType>
void Scene::RemoveComponent(const std::shared_ptr<Entity>& entity)
{
    auto componentTypeHash = std::type_index(typeid(ComponentType));
    std::unordered_map<std::type_index, size_t>& componentMap = m_EntityToComponentMap[entity];
    auto iterator = componentMap.find(componentTypeHash);

    if (iterator == componentMap.end())
    {
        Logger::Warn("Could not remove a non-existent component.");
        return;
    }

    size_t index = iterator->second;
    ComponentContainer<ComponentType>& componentContainer = GetOrCreateComponentContainer<ComponentType>();

    if (index < componentContainer.Size() - 1)
    {
        componentContainer[index] = std::move(componentContainer.Back());
        m_EntityToComponentMap[componentContainer[index].second.lock()][componentTypeHash] = index;
    }

    componentContainer.PopBack();
    componentMap.erase(iterator);
}

template<typename ComponentType>
ComponentContainer<ComponentType>& Scene::GetOrCreateComponentContainer() const
{
    auto componentTypeHash = std::type_index(typeid(ComponentType));
    auto iterator = m_ComponentContainerMap.find(componentTypeHash);

    if (iterator == m_ComponentContainerMap.end())
    {
        iterator = m_ComponentContainerMap
            .emplace(componentTypeHash, std::make_unique<ComponentContainer<ComponentType>>())
            .first;
    }

    return static_cast<ComponentContainer<ComponentType>&>(*iterator->second);
}
}
