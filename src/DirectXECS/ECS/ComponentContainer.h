#pragma once

namespace DirectXECS::ECS {
class Component;

class AbstractComponentContainer {};

template<typename ComponentType> requires std::is_base_of<Component, ComponentType>::value
class ComponentContainer : public AbstractComponentContainer {
public:
    std::vector<ComponentType> *GetVector();

private:
    std::vector<ComponentType> componentVector_;
};
}
