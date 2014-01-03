#ifndef HYBRID_ECS_GROUP_MANAGER_HPP
#define HYBRID_ECS_GROUP_MANAGER_HPP

#include <Managers/Manager.hpp>
#include <Utils/Types.hpp>
#include <Core/EntityInfo.hpp>
#include <Core/Entity.hpp>

namespace Hybrid
{
    namespace ECS
    {
        class GroupManager : public Manager
        {
            public:
                GroupManager();
                virtual ~GroupManager();

                GroupManager(GroupManager const &) = delete;
                GroupManager & operator=(GroupManager const &) = delete;

                // Add the entity to a group
                void addToGroup(EntityInfo & e, std::string const & group);

                // Remove the entity from a group
                void removeFromGroup(EntityInfo & e, std::string const & group);
                
                // Remove the entity from all its groups
                void removeFromAllGroups(EntityInfo & e);

                // Tells if the entity in is thee given group
                bool isInGroup(EntityInfo const & e, std::string const & group);

                // Retrieve all the entities in the given group
                Vector<Entity> const & getEntitiesByGroup(std::string const & group);

                // Get all the groups the entity is in
                //
                // /!\ Expensive call
                Vector<std::string> getGroupsByEntity(EntityInfo const & e);

                // Called when the Entity System has registered this manager
                virtual void onRegistered() override;

            private:
                GroupBit getGroupId(std::string const & group);

            protected:
                Hash<std::string, GroupBit> m_groupIds;
                Vector<std::string> m_groupNames;
                Vector<Vector<Entity>> m_entitiesByGroup;
                GroupBit m_nextGroupId;
        };
    }
}

#endif //HYBRID_ECS_GROUP_MANAGER_HPP
