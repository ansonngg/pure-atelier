#include "Entity.h"

namespace DirectEcs
{
Entity::Entity(const std::shared_ptr<Scene>& scene, uint32_t id) : m_pScene(scene), m_Id(id)
{
}

uint32_t Entity::GetId() const
{
    return m_Id;
}
}
