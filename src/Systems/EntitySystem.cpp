#include <Systems/EntitySystem.hpp>

namespace Hybrid
{ 
    namespace ECS 
    { 
        EntitySystemBit BaseEntitySystemId::s_nextBitId = 0; 
        BaseEntitySystem::BaseEntitySystem():
            System(), m_componentBitMask(), m_entities(), m_lookup()
        {

        }

        BaseEntitySystem::~BaseEntitySystem()
        {

        }

        void BaseEntitySystem::update()
        {
            if(shouldProcess())
            {
                onPreProcessing();
                processEntities(m_entities);
                onPostProcessing();
            }
        }


        ComponentBitMask const & BaseEntitySystem::componentBitMask() const
        {
            return m_componentBitMask;
        }

        void BaseEntitySystem::onRegistered()
        {
            assert(m_world != nullptr);

            onRegisteredEnd();
        }


        void BaseEntitySystem::onRegisteredEnd()
        {

        }

        void BaseEntitySystem::registerComponents()
        {

        }

        void BaseEntitySystem::onEntityAdded(Entity const &)
        {

        }

        void BaseEntitySystem::onEntityRemoved(Entity const &)
        {

        }

        bool BaseEntitySystem::shouldProcess()
        {
            return this->isEnabled();
        }

        void BaseEntitySystem::onPreProcessing()
        {

        }

        void BaseEntitySystem::onPostProcessing()
        {

        }
    }
}

