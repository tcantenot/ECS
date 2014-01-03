#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "../components/Position.hpp"
#include "../components/Velocity.hpp"

#include <HybridECS.hpp>


class MovementSystem : public Hybrid::ECS::EntitySystem<MovementSystem>
{
    using Entity = Hybrid::ECS::Entity;

    public:
        void registerComponents()
        {
            // Register required components
            registerComponent<Position>();
            registerComponent<Velocity>();
        }

        virtual void processEntities(Hybrid::ECS::Vector<Entity> & entities) override
        {
            auto & positions        = m_world->getAllComponents<Position>();
            auto const & velocities = m_world->getAllComponents<Velocity>();

            auto const delta = m_world->getDelta();

            for(auto const & e : entities)
            {
                auto & position       = positions[e.id()];
                auto const & velocity = velocities[e.id()];

                // Update position
                position.x += velocity.x * delta;
                position.y += velocity.y * delta;
            }
        }

        // Deprecated : Too much cache misses
        //virtual void process(Entity const & e)
        //{
            //auto position = m_world->getComponent<Position>(e);
            //auto velocity = m_world->getComponent<Velocity>(e);

            //position->x += velocity->x * m_world->getDelta();
            //position->y += velocity->y * m_world->getDelta();
        //}
};

#endif //MOVEMENT_SYSTEM_HPP
