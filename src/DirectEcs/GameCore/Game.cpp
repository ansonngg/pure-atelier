#include "Game.h"

#include "Scene.h"

#include "Component/MeshComponent.h"
#include "Component/TransformComponent.h"
#include "GameCore/Entity.h"
#include "Object/Mesh.h"

namespace DirectEcs
{
Game::Game() : m_Scene(std::make_shared<Scene>())
{
}

void Game::Init()
{
    std::shared_ptr<Entity> entity = m_Scene->CreateEntity();
    Vector3 zeroVector(0.0f, 0.0f, 0.0f);
    entity->AddComponent<TransformComponent>(zeroVector, zeroVector, zeroVector);
    entity->AddComponent<MeshComponent>(std::make_shared<Mesh>(""));
}

void Game::Update(double deltaSecond)
{
    m_Scene->Update(deltaSecond);
}
}
