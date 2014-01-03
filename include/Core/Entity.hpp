#ifndef HYBRID_ECS_ENTITY_HPP
#define HYBRID_ECS_ENTITY_HPP

#include <iosfwd>

#include <Core/EntityInfo.hpp>
#include <Utils/Types.hpp>


namespace Hybrid
{ 
    namespace ECS 
    {
        class Entity
        {
            public:
                Entity();

                explicit Entity(EntityInfo const & other);

                Entity & operator=(EntityInfo const & other);

                bool operator==(Entity const & other) const;
                bool operator!=(Entity const & other) const;

                bool operator==(EntityInfo const & other) const;
                bool operator!=(EntityInfo const & other) const;

                EntityId id() const;
                EntityId uniqueId() const;

                friend std::ostream & operator<<(std::ostream & os, Entity const & e);

                static bool IsNotInitialized(Entity const & e);

            protected:
                friend class EntityManager;
                friend class SystemManager;

                Entity(EntityId id, EntityId uniqueId);

            private:
                EntityId m_id;
                EntityId m_uniqueId;
        };
    }
}

#endif //HYBRID_ECS_ENTITY_HPP
