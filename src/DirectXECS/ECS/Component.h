#pragma once

namespace DirectXECS::ECS {
class Entity;

class Component {
public:
    explicit Component(const std::shared_ptr<Entity> &entity);
    [[nodiscard]] Entity *GetEntity() const;

private:
    std::weak_ptr<Entity> entity_;
};
}
