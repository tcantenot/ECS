#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "../components/Ball.hpp"

#include <HybridECS.hpp>

using namespace Hybrid::ECS; // BAD ...

class CollisionSystem : public EntityProcessingSystem<CollisionSystem>
{
    public:
        CollisionSystem(): EntityProcessingSystem()
        {

        }

        void registerComponents()
        {
            // Register required components
            registerComponent<Ball>();
        }

        virtual void onPreProcessing()
        {
            m_w = m_world->getValue<int>("window_width");
            m_h = m_world->getValue<int>("window_height");
        }

        virtual void process(Entity const & e)
        {
            const float DAMPENING_VERTICAL   = 1; //0.95f;
            const float DAMPENING_HORIZONTAL = 1; //0.95f;

            auto b = m_world->getComponent<Ball>(e);
            // If we hit a wall, bounce and apply a dampening coefficient
            if ((b->x - b->r < 0 && b->x < 0) || (b->x + b->r > m_w && b->x > 0))
            {
                b->vx = -b->vx * DAMPENING_HORIZONTAL;
            }

            if ((b->y - b->r < 0 && b->y < 0) || (b->y + b->r > m_h && b->y > 0))
            {
                b->vy = -b->vy * DAMPENING_VERTICAL;
            }
        }

    private:
        int m_w, m_h;
};

#endif //COLLISION_SYSTEM_HPP
