#include "Entity.h"

namespace DirectEcs
{
uint32_t Entity::GetId() const
{
    return m_Id;
}

Entity::Entity(const std::shared_ptr<Scene>& scene, uint32_t id) : m_Scene(scene), m_Id(id)
{
}
}
