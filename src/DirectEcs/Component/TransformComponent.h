#pragma once

namespace DirectEcs
{
class TransformComponent
{
public:
    TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale);

private:
    Vector3 position_;
    Vector3 rotation_;
    Vector3 scale_;
};
}
