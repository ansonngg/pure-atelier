#pragma once

namespace DirectEcs
{
class Scene;

class Game
{
public:
    Game();
    void Init();
    void Update(double deltaSecond);

private:
    std::shared_ptr<Scene> m_Scene;
};
}
