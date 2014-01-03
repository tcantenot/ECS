#include <Managers/EntityManager.hpp>
#include <Managers/SystemManager.hpp>
#include <Managers/GroupManager.hpp>
#include <Core/World.hpp>
#include <Utils/Constants.hpp>

#include <cassert>

namespace Hybrid
{ 
    namespace ECS 
    { 
        EntityManager::EntityManager():
            Manager(),
            m_entityInfos(), /*m_alive(),*/ 
            m_dead(), m_removed(), m_count(0),
            m_nextEntityId(0), 
            m_nextUniqueId(Constants::ENTITY_FIRST_UNIQUE_ID),
            m_totalCreated(0), m_totalRemoved(0)
        {

        }


        EntityManager::~EntityManager()
        {

        }

        auto EntityManager::newEntity() -> Entity
        {
            auto newId = EntityId(0);

            if(!m_dead.empty()) // Are there any unused id ?
            {
                newId = m_dead.back(); // Yes -> recycle id
                m_dead.pop_back();
            }
            else
            {
                newId = m_nextEntityId++;
                auto info = EntityInfo{ };
                info.m_id  = newId;

                if(newId >= m_entityInfos.capacity())
                {
                    m_entityInfos.reserve(2 * newId + 1);
                }

                if(m_entityInfos.size() == newId)
                {
                    m_entityInfos.resize(newId + 1);
                }

                m_entityInfos[newId] = info;
            }

            assert(m_nextUniqueId < Constants::MAX_ENTITIES);

            auto newUniqueId = m_nextUniqueId++;

            auto newEntity = Entity(newId, newUniqueId);

            m_entityInfos[newId].m_alive = true;
            m_entityInfos[newId].m_uniqueId = newUniqueId;

            //m_alive.push_back(newEntity);

            ++m_totalCreated;
            ++m_count;

            return newEntity;
        }

        bool EntityManager::isAlive(Entity const & e) const
        {
            assert(e.m_id < m_entityInfos.size());

            auto const & info = m_entityInfos[e.m_id];

            return info.m_alive && (e.m_uniqueId == info.m_uniqueId);
        }

        EntityInfo const & EntityManager::getEntityInfo(Entity const & e) const
            throw(DeadEntityException, NotInitializedEntityException)
        {
            return this->getEntityInfo(e);
        }

        EntityInfo & EntityManager::getEntityInfo(Entity const & e)
            throw(DeadEntityException, NotInitializedEntityException)
        {
            #ifdef HYBRID_ECS_FAST
                return m_entityInfos[e.m_id];
            #else
                if(this->isAlive(e))
                {
                    return m_entityInfos[e.m_id];
                }

                if(Entity::IsNotInitialized(e))
                {
                    throw NotInitializedEntityException();
                }

                throw DeadEntityException(e);
            #endif
        }


        void EntityManager::queueForRemoval(Entity const & e)
            throw(DeadEntityException, NotInitializedEntityException) 
        {
            this->queueForRemoval(this->getEntityInfo(e));
        }

        //std::vector<Entity> const & EntityManager::alive() const
        //{
            //return m_alive;
        //}

        std::size_t EntityManager::count() const
        {
            return m_count;
        }

        std::size_t EntityManager::totalCreated() const
        {
            return m_totalCreated;
        }

        std::size_t EntityManager::totalRemoved() const
        {
            return m_totalRemoved;
        }

        void EntityManager::onRegistered()
        {

        }

        void EntityManager::onLoopStart()
        {
            removeDeadEntities();
        }


        void EntityManager::queueForRemoval(EntityInfo & info)
        {
            if(!info.m_waitingForRemoval)
            {
                info.m_waitingForRemoval = true;

                m_removed.push_back(info.m_id);
            }
        }

        void EntityManager::remove(Entity const & e)
            throw(DeadEntityException, NotInitializedEntityException)
        {
            this->remove(this->getEntityInfo(e));
        }

        void EntityManager::remove(EntityInfo & info)
        {
            if(info.m_alive)
            {
                assert(m_world != nullptr);
                assert(m_world->hasManager<SystemManager>());
                assert(m_world->hasManager<GroupManager>());

                info.m_waitingForRemoval = false;
                info.m_alive = false;

                //for(auto it = m_alive.begin(); it != m_alive.end(); ++it)
                //{
                    //if(it->uniqueId() == info.uniqueId())
                    //{
                        //m_alive[info.m_id] = m_alive[m_alive.size() - 1];
                        //m_alive.pop_back();
                        //break;
                    //}
                //}

                m_dead.push_back(info.m_id);

                info.m_componentBitMask.reset();

                // Remove entity from systems
                // (m_componentBitMask does not match any system now)
                m_world->getManager<SystemManager>()->remove(info);
                info.m_systemBitMask.reset();

                // Remove entity from groups
                m_world->getManager<GroupManager>()->removeFromAllGroups(info);
                info.m_groupBitMask.reset();

                --m_count;
                ++m_totalRemoved;
            }
        }


        void EntityManager::removeDeadEntities()
        {
            for(auto i : m_removed)
            {
                assert(i < m_entityInfos.size());
                remove(m_entityInfos[i]);
            }
            m_removed.clear();
        }
    }
}
