#ifndef HYBRID_ECS_ENTITY_PROCESSING_SYSTEM_HPP
#define HYBRID_ECS_ENTITY_PROCESSING_SYSTEM_HPP

#include <Systems/EntitySystem.hpp>
#include <Core/Entity.hpp>

namespace Hybrid
{ 
    namespace ECS 
    {
        template <typename T>
        class EntityProcessingSystem : public EntitySystem<T>
        {
            public:
                EntityProcessingSystem();
                virtual ~EntityProcessingSystem();

                void processEntities(Vector<Entity> & entities) override;

                virtual void process(Entity const & e) = 0;

            //Overridable
            //protected:
                ////Called at the end of onRegistered
                //virtual void onRegisteredEnd() override;

                ////Register required components here
                //virtual void registerComponents() override;

                //virtual void onEntityAdded(Entity const & e) override;

                //virtual void onEntityRemoved(Entity const & e) override;

                //virtual bool shouldProcess() override;

                //virtual void onPreProcessing() override;

                //virtual void onPostProcessing() override;
        };

        #include "EntityProcessingSystem.inl"
    }
}

#endif //HYBRID_ECS_ENTITY_PROCESSING_SYSTEM_HPP
