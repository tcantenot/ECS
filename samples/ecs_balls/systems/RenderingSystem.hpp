#ifndef RENDERING_SYSTEM_HPP
#define RENDERING_SYSTEM_HPP

#include "../components/Ball.hpp"

#include <HybridECS.hpp>

#include "../graphics/PixelBuffer.h"

class RenderingSystem : public Hybrid::ECS::EntityProcessingSystem<RenderingSystem>
{
    using Entity = Hybrid::ECS::Entity;

    public:
        RenderingSystem(PixelBuffer* target)
            : m_target(target)
        {

        }

        void registerComponents()
        {
            // Register required components
            registerComponent<Ball>();
        }

        // Process entities
        virtual void process(Entity const & e)
        {
            // Get properties of circle
            auto b = m_world->getComponent<Ball>(e);

            // Draw circle with properties
            m_target->drawCircle(b->c.color, b->r, b->x, b->y);
        }

    private:
        PixelBuffer* m_target;
};

#endif //RENDERING_SYSTEM_HPP
