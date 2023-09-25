#pragma once

namespace DirectXECS::ECS {
class Entity {
public:
    [[nodiscard]] uint32_t GetId() const;

private:
    uint32_t id_;
};
}
