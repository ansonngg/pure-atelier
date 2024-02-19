#pragma once

#include "ComponentContainer.h"

#include "Util/Concept.h"

namespace DirectEcs
{
class IComponent;
class Scene;

template<Derived<IComponent>... ComponentTypes>
class ComponentBatch
{
public:
    class Iterator
    {
    public:
        explicit Iterator(const ComponentBatch& componentBatch);
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
        std::tuple<ComponentTypes& ...> operator*() const;

    private:
        const ComponentBatch& m_ComponentBatch;
        std::array<std::size_t, sizeof...(ComponentTypes)> m_ComponentIndices;
    };

    ComponentBatch(
        const std::unordered_map<std::shared_ptr<Entity>, std::unordered_map<std::type_index, std::size_t>>&
        entityToComponentMap,
        const std::unordered_map<std::type_index, std::shared_ptr<IComponentContainer>>& componentContainerMap
    );
    Iterator begin();
    Iterator end() const;

private:
    std::tuple<ComponentContainer<ComponentTypes>& ...> m_ContainerTuple;
    const std::unordered_map<std::shared_ptr<Entity>, std::unordered_map<std::type_index, std::size_t>>&
        m_EntityToComponentMap;
    std::size_t m_SmallestContainerIndex = 0;
    std::array<std::type_index, sizeof...(ComponentTypes)> m_ComponentTypeHashes;
};

template<Derived<IComponent>... ComponentTypes>
ComponentBatch<ComponentTypes...>::ComponentBatch(
    const std::unordered_map<std::shared_ptr<Entity>, std::unordered_map<std::type_index, std::size_t>>&
    entityToComponentMap,
    const std::unordered_map<std::type_index, std::shared_ptr<IComponentContainer>>& componentContainerMap
)
    : m_EntityToComponentMap(entityToComponentMap)
{
    std::tuple<ComponentContainer<ComponentTypes>* ...> containerPointerTuple;
    std::size_t index = 0;

    ([&]
    {
        m_ComponentTypeHashes[index] = std::type_index(typeid(ComponentTypes));
        auto iterator = componentContainerMap.find(m_ComponentTypeHashes[index]);
        std::get<index++>(containerPointerTuple) = iterator == componentContainerMap.end()
            ? nullptr
            : static_cast<ComponentContainer<ComponentTypes>*>(iterator->second.get());
    }, ...);

    std::size_t containerCount = sizeof...(ComponentTypes);
    std::size_t minSize = SIZE_MAX;

    for (std::size_t i = 0; i < containerCount; ++i)
    {
        auto containerPointer = std::get<i>(containerPointerTuple);

        if (!containerPointer)
        {
            m_SmallestContainerIndex = containerCount;
            return;
        }

        std::get<i>(m_ContainerTuple) = *containerPointer;
        std::size_t size = containerPointer->Size();

        if (size < minSize)
        {
            m_SmallestContainerIndex = i;
            minSize = size;
        }
    }
}

template<Derived<IComponent>... ComponentTypes>
ComponentBatch<ComponentTypes...>::Iterator::Iterator(const ComponentBatch& componentBatch)
    : m_ComponentBatch(componentBatch)
{
    m_ComponentIndices.fill(-1);
}

template<Derived<IComponent>... ComponentTypes>
typename ComponentBatch<ComponentTypes...>::Iterator& ComponentBatch<ComponentTypes...>::Iterator::operator++()
{
    std::size_t mainContainerIndex = m_ComponentBatch.m_SmallestContainerIndex;
    auto& mainContainer = std::get<mainContainerIndex>(m_ComponentBatch.m_ContainerTuple);
    std::size_t mainContainerSize = mainContainer.Size();
    std::size_t & mainComponentIndex = m_ComponentIndices[mainContainerIndex];

    while (++mainComponentIndex < mainContainerSize)
    {
        std::unordered_map<std::type_index, std::size_t>& componentMap =
            m_ComponentBatch.m_EntityToComponentMap[mainContainer[mainComponentIndex].second.lock()];
        bool isValid = true;

        for (std::size_t i = 0; i < m_ComponentIndices.size(); ++i)
        {
            if (i == mainContainerIndex)
            {
                continue;
            }

            auto iterator = componentMap.find(m_ComponentBatch.m_ComponentTypeHashes[i]);

            if (iterator == componentMap.end())
            {
                isValid = false;
                break;
            }

            m_ComponentIndices[i] = iterator->second;
        }

        if (isValid)
        {
            break;
        }
    }

    if (mainComponentIndex == mainContainerSize)
    {
        m_ComponentIndices.fill(-1);
    }

    return *this;
}

template<Derived<IComponent>... ComponentTypes>
bool ComponentBatch<ComponentTypes...>::Iterator::operator!=(const Iterator& other) const
{
    return m_ComponentIndices[0] != other.m_ComponentIndices[0];
}

template<Derived<IComponent>... ComponentTypes>
std::tuple<ComponentTypes& ...> ComponentBatch<ComponentTypes...>::Iterator::operator*() const
{
    std::tuple<ComponentTypes& ...> result;

    for (std::size_t i = 0; i < m_ComponentIndices.size(); ++i)
    {
        std::get<i>(result) = std::get<i>(m_ComponentBatch.m_ContainerTuple)[m_ComponentIndices[i]].first;
    }

    return result;
}

template<Derived<IComponent>... ComponentTypes>
typename ComponentBatch<ComponentTypes...>::Iterator ComponentBatch<ComponentTypes...>::begin()
{
    return m_SmallestContainerIndex == sizeof...(ComponentTypes) ? end() : ++Iterator(*this);
}

template<Derived<IComponent>... ComponentTypes>
typename ComponentBatch<ComponentTypes...>::Iterator ComponentBatch<ComponentTypes...>::end() const
{
    return Iterator(*this);
}
}
