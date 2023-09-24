#pragma once

#include "PCH.h"

namespace DirectXECS::ECS {
class Entity {
public:
    [[nodiscard]] uint32_t GetId() const;

private:
    uint32_t id_;
};
}
