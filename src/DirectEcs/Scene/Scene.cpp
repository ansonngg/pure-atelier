#include "Scene.h"

#include "Component.h"
#include "Entity.h"
#include "ISystem.h"

namespace DirectEcs
{
void Scene::Update(double deltaSecond)
{
    for (auto& system : m_Systems)
    {
        system->Update(deltaSecond);
    }
}

std::shared_ptr<Entity> Scene::CreateEntity()
{
    return m_EntityToComponentMap.emplace(
        std::make_shared<Entity>(shared_from_this(), m_NextEntityId++),
        std::unordered_map<std::type_index, size_t>()
    ).first->first;
}
}
