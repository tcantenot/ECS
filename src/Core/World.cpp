#include <Core/World.hpp>


namespace Hybrid
{
    namespace ECS
    {
        World::World():
            m_metaManager(this),
            m_entityManager(new EntityManager),
            m_componentManager(new ComponentManager),
            m_systemManager(new SystemManager),
            m_groupManager(new GroupManager),
            //m_tagManager(new TagManager),
            m_variableManager(new VariableManager),
            m_delta(0)
            
        {
            m_metaManager.registerManager(m_entityManager);
            m_metaManager.registerManager(m_componentManager);
            m_metaManager.registerManager(m_systemManager);
            m_metaManager.registerManager(m_groupManager);
            //m_metaManager.registerManager(m_tagManager);
            m_metaManager.registerManager(m_variableManager);
            

        }

        World::~World()
        {

        }

        // On loop start :
        //  - removes "queued for removal" entities
        //  - queues updated entities into their corresponding systems
        void World::onLoopStart()
        {
            m_entityManager->onLoopStart();
            m_systemManager->onLoopStart();
        }

        void World::setDelta(Time delta)
        {
            m_delta = delta;
        }

        World::Time World::getDelta()
        {
            return m_delta;
        }

        void World::update()
        {
            m_systemManager->update();
        }

        // ---------------------------------------------------------------------
        //  ENTITY
        // ---------------------------------------------------------------------

        Entity World::newEntity()
        {
            return m_entityManager->newEntity();
        }

        void World::remove(Entity const & e, bool now)
            throw(DeadEntityException, NotInitializedEntityException)
        {
            if(now) {
                m_entityManager->remove(e);
            }
            else {
                m_entityManager->queueForRemoval(e);
            }
        }

        bool World::isAlive(Entity const & e)
        {
            return m_entityManager->isAlive(e);
        }

        EntityInfo const & World::getEntityInfo(Entity const & e)
            throw(DeadEntityException, NotInitializedEntityException)
        {
            return m_entityManager->getEntityInfo(e);
        }

        std::size_t World::count() const
        {
            return m_entityManager->count();
        }

        std::size_t World::totalCreated() const
        {
            return m_entityManager->totalCreated();
        }

        std::size_t World::totalRemoved() const
        {
            return m_entityManager->totalRemoved();
        }


        // ---------------------------------------------------------------------
        //  COMPONENT
        // ---------------------------------------------------------------------

        void World::removeAllComponents(Entity const & e) const
            throw(DeadEntityException, NotInitializedEntityException)
        {
            auto & info = m_entityManager->getEntityInfo(e);
            
            m_componentManager->removeAllComponents(info);

            // Remove entity from the systems
            m_systemManager->queueForUpdate(e, info);
        }

        // ---------------------------------------------------------------------
        //  MANAGER
        // ---------------------------------------------------------------------

        // ---------------------------------------------------------------------
        //  SYSTEM
        // ---------------------------------------------------------------------

        void World::updateSystems()
        {
            m_systemManager->updateSystems();
        }

        void World::updateEntitySystems()
        {
            m_systemManager->updateEntitySystems();
        }

        // ---------------------------------------------------------------------
        //  GROUP
        // ---------------------------------------------------------------------
         
        void World::addToGroup(Entity const & e, std::string const & group)
            throw(DeadEntityException, NotInitializedEntityException)
        {
            auto & info = m_entityManager->getEntityInfo(e);

            m_groupManager->addToGroup(info, group);
        }
            
        void World::removeFromGroup(Entity const & e, std::string const & group)
            throw(DeadEntityException, NotInitializedEntityException)
        {

            auto & info = m_entityManager->getEntityInfo(e);

            m_groupManager->removeFromGroup(info, group);
        }


        void World::removeFromAllGroups(Entity const & e)
            throw(DeadEntityException, NotInitializedEntityException)
        {
            auto & info = m_entityManager->getEntityInfo(e);

            m_groupManager->removeFromAllGroups(info);
        }

        bool World::isInGroup(Entity const & e, std::string const & group)
            throw(DeadEntityException, NotInitializedEntityException)
        {
            auto & info = m_entityManager->getEntityInfo(e);

            return m_groupManager->isInGroup(info, group);
        }

        Vector<Entity> const & World::getEntitiesByGroup(std::string const & g)
        {
            return m_groupManager->getEntitiesByGroup(g);
        }

        Vector<std::string> World::getGroupsByEntity(Entity const & e)
            throw(DeadEntityException, NotInitializedEntityException)
        {
            auto & info = m_entityManager->getEntityInfo(e);

            return std::move(m_groupManager->getGroupsByEntity(info));
        }


        // ---------------------------------------------------------------------
        //  TAG
        // ---------------------------------------------------------------------

        // ---------------------------------------------------------------------
        //  VARIABLE
        // ---------------------------------------------------------------------
    }
}
