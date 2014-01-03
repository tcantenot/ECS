#ifndef HYBRID_ECS_ENTITY_INFO
#define HYBRID_ECS_ENTITY_INFO

#include <iosfwd>

#include <Utils/Types.hpp>

namespace Hybrid
{ 
    namespace ECS 
    { 
        class EntityInfo
        {
            public:
                EntityInfo();

                EntityId id() const;
                EntityId uniqueId() const;
                bool alive() const;

                ComponentBitMask const & componentBitMask() const;
                EntitySystemBitMask const & systemBitMask() const;
                GroupBitMask const & groupBitMask() const;

                friend std::ostream & operator<<(std::ostream & os, EntityInfo const & info);

            protected:
                friend class World;

                friend class EntityManager;
                friend class SystemManager;
                friend class ComponentManager;
                friend class GroupManager;
                template <typename T>
                friend class EntitySystem;

                void addComponent(ComponentBit bit);
                void removeComponent(ComponentBit bit);
                void removeAllComponents();

                void addSystem(EntitySystemBit const & bit);
                void removeSystem(EntitySystemBit const & bit);

                bool matchesComponentBitMask(ComponentBitMask const & mask) const;
                bool matchesEntitySystemBitMask(EntitySystemBitMask const & mask) const;
                bool matchesSystemBit(EntitySystemBit const & bit) const;

            protected:
                ComponentBitMask    m_componentBitMask;
                EntitySystemBitMask m_systemBitMask;
                GroupBitMask        m_groupBitMask;

                bool m_alive;
                bool m_waitingForRemoval;
                bool m_waitingForUpdate;

            private:
                EntityId m_id;
                EntityId m_uniqueId;
        };
    }
}

#endif //HYBRID_ECS_ENTITY_INFO
