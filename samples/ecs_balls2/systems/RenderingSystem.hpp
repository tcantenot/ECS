#ifndef RENDERING_SYSTEM_HPP
#define RENDERING_SYSTEM_HPP

#include "../components/Position.hpp"
#include "../components/Radius.hpp"
#include "../components/Color.hpp"

#include <HybridECS.hpp>

#include "../graphics/PixelBuffer.h"

class RenderingSystem : public Hybrid::ECS::EntitySystem<RenderingSystem>
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
            registerComponent<Position>();
            registerComponent<Radius>();
            registerComponent<Color>();
        }

        virtual void processEntities(Hybrid::ECS::Vector<Entity> & entities) override
        {
            auto const & positions = m_world->getAllComponents<Position>();
            auto const & radiuses  = m_world->getAllComponents<Radius>();
            auto const & colors    = m_world->getAllComponents<Color>();

            for(auto const & e : entities)
            {
                auto const & position = positions[e.id()];
                auto const & radius   = radiuses[e.id()];
                auto const & color    = colors[e.id()];

                m_target->drawCircle(color.color, radius.radius, position.x, position.y);
            }
        }


        // Deprecated : Too much cache misses
/*        virtual void process(Entity const & e)*/
        //{
            //// Get properties of circle
            //auto position = m_world->getComponent<Position>(e);
            //auto radius   = m_world->getComponent<Radius>(e);
            //auto color    = m_world->getComponent<Color>(e);

            //// Draw circle with properties
            //m_target->drawCircle(color->color, radius->radius, position->x, position->y);
        //}

    private:
        PixelBuffer* m_target;
};

#endif //RENDERING_SYSTEM_HPP
