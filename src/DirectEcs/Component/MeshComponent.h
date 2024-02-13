#pragma once

namespace DirectEcs
{
class Mesh;

class MeshComponent
{
public:
    explicit MeshComponent(const std::shared_ptr<Mesh>& mesh);

private:
    std::shared_ptr<Mesh> mesh_;
};
}
