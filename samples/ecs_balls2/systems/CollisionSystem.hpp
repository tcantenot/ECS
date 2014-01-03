#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Radius.hpp"

#include <HybridECS.hpp>

using namespace Hybrid::ECS; // BAD ...

class CollisionSystem : public EntitySystem<CollisionSystem>
{
    public:
        CollisionSystem(): EntitySystem()
        {

        }

        void registerComponents()
        {
            // Register required components
            registerComponent<Position>();
            registerComponent<Velocity>();
            registerComponent<Radius>();
        }

        virtual void onPreProcessing()
        {
            m_w = m_world->getValue<int>("window_width");
            m_h = m_world->getValue<int>("window_height");
        }

        virtual void processEntities(Hybrid::ECS::Vector<Entity> & entities) override
        {
            const float DAMPENING_VERTICAL   = 1; //0.95f;
            const float DAMPENING_HORIZONTAL = 1; //0.95f;

            auto const & positions  = m_world->getAllComponents<Position>();
            auto & velocities       = m_world->getAllComponents<Velocity>();
            auto const & radiuses   = m_world->getAllComponents<Radius>();

            for(auto const & e : entities)
            {
                auto const & position = positions[e.id()];
                auto & velocity       = velocities[e.id()];
                auto const & radius   = radiuses[e.id()];

                // If we hit a wall, bounce and apply a dampening coefficient
                if ((position.x - radius.radius < 0 && velocity.x < 0) || (position.x + radius.radius > m_w && velocity.x > 0))
                {
                    velocity.x = -velocity.x * DAMPENING_HORIZONTAL;
                }
                if ((position.y - radius.radius < 0 && velocity.y < 0) || (position.y + radius.radius > m_h && velocity.y > 0))
                {
                    velocity.y = -velocity.y * DAMPENING_VERTICAL;
                }
            }
        }


        // Deprecated : Too much cache misses
        virtual void process(Entity const & e)
        {
            const float DAMPENING_VERTICAL   = 1; //0.95f;
            const float DAMPENING_HORIZONTAL = 1; //0.95f;

            auto position = m_world->getComponent<Position>(e);
            auto velocity = m_world->getComponent<Velocity>(e);
            auto radius   = m_world->getComponent<Radius>(e);

            // If we hit a wall, bounce and apply a dampening coefficient
            if ((position->x - radius->radius < 0 && velocity->x < 0) || (position->x + radius->radius > m_w && velocity->x > 0))
            {
                velocity->x = -velocity->x * DAMPENING_HORIZONTAL;
            }
            if ((position->y - radius->radius < 0 && velocity->y < 0) || (position->y + radius->radius > m_h && velocity->y > 0))
            {
                velocity->y = -velocity->y * DAMPENING_VERTICAL;
            }
        }

    private:
        int m_w, m_h;
};

#endif //COLLISION_SYSTEM_HPP
