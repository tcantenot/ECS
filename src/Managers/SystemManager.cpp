#include <Managers/SystemManager.hpp>
#include <Core/World.hpp>
#include <Core/Entity.hpp>


namespace Hybrid
{
    namespace ECS
    {
        SystemManager::SystemManager(): 
            Manager(), m_systems(), m_entitySystems(),
            m_updatedEntities(), m_entityManager()
        {
            m_entitySystems.reserve(0x10);
            m_updatedEntities.reserve(0x10);
        }

        SystemManager::~SystemManager()
        {

        }

        void SystemManager::onRegistered()
        {
            assert(m_world != nullptr);

            m_entityManager = m_world->getManager<EntityManager>();
            assert(m_entityManager != nullptr);
        }

        void SystemManager::onLoopStart()
        {
            for(auto & e : m_updatedEntities)
            {
                this->updateEntity(e);
            }

            m_updatedEntities.clear();
        }

        void SystemManager::update()
        {
            updateSystems();
            updateEntitySystems();
        }

        void SystemManager::updateSystems()
        {
            for(auto & s : m_systems)
            {
                assert(s.second != nullptr);
                if(s.second->isEnabled()) s.second->update();
            }
        }

        void SystemManager::updateEntitySystems()
        {
            for(auto & s : m_entitySystems)
            {
                if(s != nullptr && s->isEnabled()) s->update();
            }
        }

        void SystemManager::queueForUpdate(Entity const & e)
        {
            auto & info = m_world->getManager<EntityManager>()->getEntityInfo(e);
            this->queueForUpdate(e, info);
        }

        void SystemManager::queueForUpdate(Entity const & e, EntityInfo & info)
        {
            if(!info.m_waitingForUpdate)
            {
                info.m_waitingForUpdate = true;

                if(m_updatedEntities.size() + 1 == m_updatedEntities.capacity())
                {
                    m_updatedEntities.reserve(2 * m_updatedEntities.size() + 1);
                }

                m_updatedEntities.push_back(e);
            }
        }

        void SystemManager::updateEntity(Entity const & e)
        {
            auto & info = m_world->getManager<EntityManager>()->getEntityInfo(e);
            this->updateEntity(e, info);
        }

        void SystemManager::updateEntity(Entity const & e, EntityInfo & info)
        {
            info.m_waitingForUpdate = false;

            // Update the list of entities of each entity system by adding or
            // removing the entity from them
            for(auto const & s : m_entitySystems)
            {
                // if:   Add the entity to the system if its component mask 
                //       matches the system component mask and if it is not
                //       already in the system
                //
                // else: Remove the entity from the system if its component
                //       mask does not match the system's one and if it is
                //       already in the system
                if(info.matchesComponentBitMask(s->componentBitMask()))
                {
                    if(!info.matchesSystemBit(s->bitId()))
                    {
                        s->addEntity(e, info);
                    }
                }
                else
                {
                    if(info.matchesSystemBit(s->bitId()))
                    {
                        s->removeEntity(e, info);
                    }
                }
            }
        }

        void SystemManager::remove(EntityInfo & info)
        {
            auto const begin = m_updatedEntities.begin();
            auto const end   = m_updatedEntities.end();

            // Remove the entity from the "queued for update" entities
            for(auto it = begin; it != end; ++it)
            {
                if(it->uniqueId() == info.uniqueId())
                {
                    assert(it->id() == info.id());
                    m_updatedEntities.erase(it);
                    break;
                }
            }

            // Remove entity from the entity systems
            this->updateEntity(Entity(info), info);
        }
    }
}
