#include "Scene.h"

#include "Entity.h"

#include "System/RenderSystem.h"

namespace DirectEcs
{
Scene::Scene()
{
    std::unique_ptr<ISystem> tempSystems[] = {
        std::make_unique<RenderSystem>()
    };
    m_Systems = std::vector<std::unique_ptr<ISystem>>(
        std::make_move_iterator(std::begin(tempSystems)),
        std::make_move_iterator(std::end(tempSystems))
    );
}

void Scene::Update(double deltaSecond)
{
    for (auto& system : m_Systems)
    {
        if (system->IsEnable())
        {
            system->Update(deltaSecond);
        }
    }
}

std::shared_ptr<Entity> Scene::CreateEntity()
{
    return m_EntityToComponentMap.emplace(
        std::make_shared<Entity>(Entity(shared_from_this(), m_NextEntityId++)),
        std::unordered_map<std::type_index, std::size_t>()
    ).first->first;
}
}
