#ifndef GRAVITY_SYSTEM_HPP
#define GRAVITY_SYSTEM_HPP

#include "../components/Velocity.hpp"

#include <HybridECS.hpp>

class GravitySystem : public Hybrid::ECS::EntitySystem<GravitySystem>
{
    using Entity = Hybrid::ECS::Entity;

    public:
        void registerComponents()
        {
            // Register required components
            registerComponent<Velocity>();
        }

        virtual void processEntities(Hybrid::ECS::Vector<Entity> & entities) override
        {
            const float GRAVITY_ACCELERATION = 500;

            auto & velocities = m_world->getAllComponents<Velocity>();

            auto const delta = m_world->getDelta();

            for(auto const & e : entities)
            {
                auto & velocity = velocities[e.id()];

                velocity.y += delta * GRAVITY_ACCELERATION;
            }
        }

        // Deprecated : Too much cache misses
        //void process(Entity const & e)
        //{
            //const float GRAVITY_ACCELERATION = 500;

            //auto velocity = m_world->getComponent<Velocity>(e);

            //velocity->y += m_world->getDelta() * GRAVITY_ACCELERATION;
        //}
};

#endif //GRAVITY_SYSTEM_HPP
