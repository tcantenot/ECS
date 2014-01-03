#include <Core/Entity.hpp>
#include <Utils/Constants.hpp>

#include <iostream>

namespace Hybrid
{ 
    namespace ECS
    {
        Entity::Entity():
            m_id(0),
            m_uniqueId(Constants::ENTITY_UNDEFINED_UNIQUE_ID)
        {

        }

        Entity::Entity(EntityInfo const & other):
            m_id(other.id()), m_uniqueId(other.uniqueId())
        {

        }

        Entity & Entity::operator=(EntityInfo const & other)
        {
            m_id = other.id();
            m_uniqueId = other.uniqueId();
            return *this;
        }

        Entity::Entity(EntityId id, EntityId uniqueId):
            m_id(id), m_uniqueId(uniqueId)
        {

        }

        bool Entity::operator==(Entity const & other) const
        {
            return m_uniqueId == other.uniqueId() && m_id == other.id();
        }

        bool Entity::operator!=(Entity const & other) const
        {
            return !this->operator==(other);
        }

        bool Entity::operator==(EntityInfo const & other) const
        {
            return m_uniqueId == other.uniqueId() && m_id == other.id();
        }

        bool Entity::operator!=(EntityInfo const & other) const
        {
            return !this->operator==(other);
        }

        EntityId Entity::id() const
        {
            return m_id;
        }

        EntityId Entity::uniqueId() const
        {
            return m_uniqueId;
        }

        std::ostream & operator<<(std::ostream & os, Entity const & e)
        {
            return os << "[" << e.m_id << ", " << e.m_uniqueId << "]";
        }

        bool Entity::IsNotInitialized(Entity const & e)
        {
            return e.m_uniqueId == Constants::ENTITY_UNDEFINED_UNIQUE_ID;
        }
    }
}
