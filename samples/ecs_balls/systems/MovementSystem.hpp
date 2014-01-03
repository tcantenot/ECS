#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "../components/Ball.hpp"

#include <HybridECS.hpp>


class MovementSystem : public Hybrid::ECS::EntityProcessingSystem<MovementSystem>
{
    using Entity = Hybrid::ECS::Entity;

    public:
        void registerComponents()
        {
            // Register required components
            registerComponent<Ball>();
        }

        virtual void process(Entity const & e)
        {
            auto b = m_world->getComponent<Ball>(e);

            // Update position
            b->x += b->vx * m_world->getDelta();
            b->y += b->vy * m_world->getDelta();
        }
};

#endif //MOVEMENT_SYSTEM_HPP
