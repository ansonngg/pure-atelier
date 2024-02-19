#pragma once

namespace DirectEcs
{
class IComponent
{
public:
    virtual ~IComponent() = default;
    [[nodiscard]] bool IsEnable() const;
    void SetIsEnable(bool isEnable);

private:
    bool m_IsEnable = true;
};
}
