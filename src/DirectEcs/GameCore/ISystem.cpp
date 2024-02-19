#include "ISystem.h"

namespace DirectEcs
{
    bool ISystem::IsEnable() const
    {
        return m_IsEnable;
    }

    void ISystem::SetIsEnable(bool isEnable)
    {
        m_IsEnable = isEnable;
    }
}
