#include "IComponent.h"

namespace DirectEcs
{
bool IComponent::IsEnable() const
{
    return m_IsEnable;
}

void IComponent::SetIsEnable(bool isEnable)
{
    m_IsEnable = isEnable;
}
}
