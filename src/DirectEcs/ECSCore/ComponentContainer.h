#pragma once

namespace DirectEcs
{
class Component;

class ComponentContainerBase
{
};

template<typename ComponentType>
requires std::is_base_of<Component, ComponentType>::value
class ComponentContainer : public ComponentContainerBase
{
public:
    std::vector<ComponentType>* GetVector();

private:
    std::vector<ComponentType> componentVector_;
};
}
