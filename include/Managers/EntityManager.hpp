#ifndef HYBRID_ECS_ENTITY_MANAGER_HPP
#define HYBRID_ECS_ENTITY_MANAGER_HPP

#include <memory>
#include <vector>

#include <Managers/Manager.hpp>
#include <Core/Entity.hpp>
#include <Core/EntityInfo.hpp>
#include <Utils/Types.hpp>
#include <Exceptions/DeadEntityException.hpp>
#include <Exceptions/NotInitializedEntityException.hpp>

namespace Hybrid
{ 
    namespace ECS 
    { 
        class EntityManager : public Manager
        {
            public:
                EntityManager();
                virtual ~EntityManager();

                EntityManager(EntityManager const &) = delete;
                EntityManager & operator=(EntityManager const &) = delete;

                // Creates a new entity and stores it in the manager
                auto newEntity() -> Entity;

                // Tells if the entity is alive in the manager
                bool isAlive(Entity const & e) const;

                // Return the info of the entity
                //  /!\ The reference may be invalidated after adding some other
                //      entities because of the reallocation of the container
                EntityInfo const & getEntityInfo(Entity const & e) const
                    throw(DeadEntityException, NotInitializedEntityException);

                // Queue the entity for removal from the system
                void queueForRemoval(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException);

                // Alive entities in the EntityManager
                //std::vector<Entity> const & alive() const;

                // Number of entities in the EntityManager
                std::size_t count() const;

                // Total number of created entities
                std::size_t totalCreated() const;

                // Total number of removed entities
                std::size_t totalRemoved() const;

                // Called when the Entity System has registered this manager
                virtual void onRegistered() override;

                // On loop start :
                //  - removes "queued for removal" entities
                void onLoopStart();

            protected:
                friend class World;
                friend class SystemManager;

                EntityInfo & getEntityInfo(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException);

                void queueForRemoval(EntityInfo & info);

                void remove(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException);

                void remove(EntityInfo & info);

                void removeDeadEntities();

                // All the entities' info
                std::vector<EntityInfo> m_entityInfos;

            private:
                // All the alives entities
                //std::vector<Entity> m_alive;

                // Index of the dead entities in m_entityInfos
                std::vector<Index> m_dead;

                // Index of queued for removal entities
                std::vector<Index> m_removed;

                // Number of entities in the manager
                std::size_t m_count;

                EntityId m_nextEntityId;
                EntityId m_nextUniqueId;

                std::size_t m_totalCreated;
                std::size_t m_totalRemoved;
        };
    }
}

#endif //HYBRID_ECS_ENTITY_MANAGER_HPP
