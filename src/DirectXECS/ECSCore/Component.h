#pragma once

namespace DirectXECS::ECSCore {
class Entity;

class Component {
public:
    [[nodiscard]] Entity *GetEntity() const;

protected:
    explicit Component(const std::shared_ptr<Entity> &entity);

private:
    std::weak_ptr<Entity> entity_;
};
}
