#include "Scene.h"

#include "Component.h"
#include "ComponentContainer.h"
#include "Entity.h"
#include "System.h"

namespace DirectEcs
{
void Scene::Update(double deltaSecond, double totalSecond)
{
    for (auto& system : systems_)
    {
        system->Update(deltaSecond, totalSecond);
    }
}

Entity* Scene::CreateEntity()
{
    return entitySet_.emplace(std::make_unique<Entity>(shared_from_this(), nextEntityId_++)).first->get();
}

template<typename ComponentType, typename... Args>
requires std::is_base_of<Component, ComponentType>::value
ComponentType* Scene::CreateComponent(std::shared_ptr<Entity> entity, Args&& ...args)
{
    size_t componentTypeHash = typeid(ComponentType).hash_code();

    // Check if a component of the required type has already existed
    auto iterator = componentContainerMap_.find(componentTypeHash);

    // Create a new container if no
    if (iterator == componentContainerMap_.end())
    {
        iterator =
            componentContainerMap_.emplace(componentTypeHash, std::make_unique<ComponentContainer<ComponentType>>())
                .first;
    }

    std::vector<ComponentType>* componentVector =
        static_cast<ComponentContainer<ComponentType>*>(iterator->second.get())->GetVector();

    // Used to sort components by the id of their owners (entities)
    struct ComponentCompare
    {
        bool operator()(const ComponentType& component, const uint32_t& entityId)
        {
            return component.GetEntity()->GetId() < entityId;
        }
    };

    auto itToInsert =
        std::lower_bound(componentVector->begin(), componentVector->end(), entity->GetId(), ComponentCompare());

    // If all entities of existing components have smaller id than the entity being added a new component
    if (itToInsert == componentVector->end())
    {
        componentVector->push_back(ComponentType(entity, std::forward<Args>(args)...));
        return &componentVector->back();
    }

    // If the entity has already had the same component
    if (itToInsert->GetEntity()->GetId() == entity->GetId())
    {
        return &*itToInsert;
    }

    // Insert the new component right before the one that has an owner of larger id
    return &*componentVector->insert(itToInsert, ComponentType(entity, std::forward<Args>(args)...));
}
}
