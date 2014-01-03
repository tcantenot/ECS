#ifndef HYBRID_ECS_ENTITY_SYSTEM_HPP
#define HYBRID_ECS_ENTITY_SYSTEM_HPP

#include <cassert>

#include <Core/Entity.hpp>
#include <Core/EntityInfo.hpp>

#include <Systems/System.hpp>

#include <Utils/Types.hpp>
#include <Utils/Constants.hpp>


namespace Hybrid 
{ 
    namespace ECS 
    { 
        struct BaseEntitySystemId
        {
            BaseEntitySystemId() = default;
            virtual ~BaseEntitySystemId() { }

            protected:
                static EntitySystemBit s_nextBitId;
        };

        struct BaseEntitySystem : public System
        {
            BaseEntitySystem();
            virtual ~BaseEntitySystem();

            void update() override;

            ComponentBitMask const & componentBitMask() const;

            virtual EntitySystemBit const & bitId() const = 0;

            virtual void processEntities(Vector<Entity> & entities) = 0;

        protected:
            friend class SystemManager;

            virtual void addEntity(Entity const & e, EntityInfo & info) = 0;

            virtual void removeEntity(Entity const & e, EntityInfo & info) = 0;

            // Called when register in world
            //  /!\ cannot be override
            //   -> override onRegisteredEnd() instead
            void onRegistered() final;

            // Called at the end of onRegistered
            virtual void onRegisteredEnd();

            // Register required components here
            virtual void registerComponents();

            virtual void onEntityAdded(Entity const & e);

            virtual void onEntityRemoved(Entity const & e);

            virtual bool shouldProcess();

            virtual void onPreProcessing();

            virtual void onPostProcessing();

            template <typename ComponentType>
            void registerComponent();


        protected:
            ComponentBitMask m_componentBitMask;
            Vector<Entity> m_entities;
            Hash<EntityId, Index> m_lookup;
        };

        template <typename T>
        class EntitySystem : public BaseEntitySystem, private BaseEntitySystemId
        {
            public:
                EntitySystem();
                virtual ~EntitySystem();

                virtual EntitySystemBit const & bitId() const override final;

            protected:
                friend class SystemManager;

                void addEntity(Entity const & e, EntityInfo & info) override final;

                void removeEntity(Entity const & e, EntityInfo & info) override final;

            public:
                static ComponentBit const BitId;
        };

        #include "EntitySystem.inl"
    }
}

#endif //HYBRID_ECS_ENTITY_SYSTEM_HPP
