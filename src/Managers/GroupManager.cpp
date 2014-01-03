#include <Managers/GroupManager.hpp>
#include <Utils/Constants.hpp>

#include <cassert>

namespace Hybrid
{ 
    namespace ECS
    {
        namespace
        {
            inline void removeFirstEqual(Vector<Entity> & v, Entity const & e)
            {
                for(std::size_t i = 0; i < v.size(); ++i)
                {
                    if(v[i] == e)
                    {
                        v[i] = v[v.size() - 1];
                        v.pop_back();
                        break;
                    }
                }
            }
        }


        GroupManager::GroupManager(): 
            m_groupIds(), m_groupNames(), 
            m_entitiesByGroup(), m_nextGroupId(0)
        {

        }

        GroupManager::~GroupManager()
        {

        }

        void GroupManager::addToGroup(EntityInfo & e, std::string const & group)
        {
            auto const groupId = this->getGroupId(group);

            if(!e.m_groupBitMask.test(groupId))
            {
                m_entitiesByGroup[groupId].push_back(Entity(e));

                e.m_groupBitMask.set(groupId);
            }
        }

        void GroupManager::removeFromGroup(EntityInfo & e, std::string const & group)
        {
            auto const groupId = this->getGroupId(group);

            if(e.m_groupBitMask.test(groupId))
            {
                auto & entities = m_entitiesByGroup[groupId];

                removeFirstEqual(entities, Entity(e));

                e.m_groupBitMask.reset(groupId);
            }
        }

        void GroupManager::removeFromAllGroups(EntityInfo & e)
        {
            for(std::size_t i = 0; i < m_nextGroupId; ++i)
            {
                if(e.m_groupBitMask.test(i))
                {
                    auto & entities = m_entitiesByGroup[i];

                    removeFirstEqual(entities, Entity(e));
                }
            }

            e.m_groupBitMask.reset();
        }

        bool GroupManager::isInGroup(EntityInfo const & e, std::string const & group)
        {
            auto const groupId = this->getGroupId(group);

            return e.m_groupBitMask.test(groupId);
        }

        Vector<Entity> const & GroupManager::getEntitiesByGroup(std::string const & group)
        {
            auto const groupId = this->getGroupId(group);

            return m_entitiesByGroup[groupId];
        }

        // /!\ Expensive call
        Vector<std::string> GroupManager::getGroupsByEntity(EntityInfo const & e)
        {
            Vector<std::string> groups;

            for(GroupBit i = 0; i < m_nextGroupId; ++i)
            {
                if(e.m_groupBitMask.test(i))
                {
                    groups.push_back(m_groupNames[i]);
                }
            }
            return std::move(groups);
        }

        void GroupManager::onRegistered()
        {

        }

        inline GroupBit GroupManager::getGroupId(std::string const & group)
        {
            auto const it = m_groupIds.find(group);

            if(it != m_groupIds.end())
            {
                return it->second;
            }

            auto const bit = m_nextGroupId++;

            auto size = std::max<size_t>(bit + 1, m_entitiesByGroup.size());
            m_entitiesByGroup.resize(size);

            assert(bit <= Constants::MAX_GROUPS);

            auto p = m_groupIds.insert(std::make_pair(group, bit));
            m_groupNames.push_back(p.first->first);

            return bit;
        }
    }
}
