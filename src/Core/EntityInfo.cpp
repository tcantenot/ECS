#include <Core/EntityInfo.hpp>
#include <Utils/Constants.hpp>

#include <iostream>
#include <iomanip>

namespace Hybrid
{ 
    namespace ECS 
    { 
        EntityInfo::EntityInfo():
            m_componentBitMask(), m_systemBitMask(), m_groupBitMask(),
            m_alive(false), m_waitingForRemoval(false), m_waitingForUpdate(false),
            m_id(0), m_uniqueId(Constants::ENTITY_UNDEFINED_UNIQUE_ID)
        {

        }
         
        EntityId EntityInfo::id() const
        {
            return m_id;
        }

        EntityId EntityInfo::uniqueId() const
        {
            return m_uniqueId;
        }

        bool EntityInfo::alive() const
        {
            return m_alive;
        }

        void EntityInfo::addComponent(ComponentBit bit)
        {
            m_componentBitMask.set(static_cast<std::size_t>(bit));
        }

        void EntityInfo::removeComponent(ComponentBit bit)
        {
            m_componentBitMask.reset(static_cast<std::size_t>(bit));
        }

        void EntityInfo::removeAllComponents()
        {
            m_componentBitMask.reset();
        }

        void EntityInfo::addSystem(EntitySystemBit const & bit)
        {
            m_systemBitMask.set(bit);
        }

        void EntityInfo::removeSystem(EntitySystemBit const & bit)
        {
            m_systemBitMask.reset(bit);
        }

        ComponentBitMask const & EntityInfo::componentBitMask() const
        {
            return m_componentBitMask;
        }

        EntitySystemBitMask const & EntityInfo::systemBitMask() const
        {
            return m_systemBitMask;
        }

        GroupBitMask const & EntityInfo::groupBitMask() const
        {
            return m_groupBitMask;
        }

        bool EntityInfo::matchesComponentBitMask(ComponentBitMask const & mask) const
        {
            return (m_componentBitMask & mask) == mask;
        }

        bool EntityInfo::matchesEntitySystemBitMask(EntitySystemBitMask const & mask) const
        {
            return (m_systemBitMask & mask) == mask;
        }

        bool EntityInfo::matchesSystemBit(EntitySystemBit const & bit) const
        {
            return m_systemBitMask.test(bit);
        }

        std::ostream & operator<<(std::ostream & os, EntityInfo const & info)
        {
            os << "[" << info.m_id << ", " << info.m_uniqueId << ", "
               << std::boolalpha
               << std::setw(5) << info.m_alive << ", "
               << std::setw(5) << info.m_waitingForUpdate  << ", "
               << std::setw(5) << info.m_waitingForRemoval << "]";
            
            return os;
        }
    }
}
