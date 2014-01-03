#ifndef GRAVITY_SYSTEM_HPP
#define GRAVITY_SYSTEM_HPP

#include "../components/Ball.hpp"

#include <HybridECS.hpp>

class GravitySystem : public Hybrid::ECS::EntityProcessingSystem<GravitySystem>
{
    using Entity = Hybrid::ECS::Entity;

    public:
        void registerComponents()
        {
            // Register required components
            registerComponent<Ball>();
        }

        void process(Entity const & e)
        {
            const float GRAVITY_ACCELERATION = 500;

            auto b = m_world->getComponent<Ball>(e);

            b->vy += m_world->getDelta() * GRAVITY_ACCELERATION;
        }
};

#endif //GRAVITY_SYSTEM_HPP
