#include <Systems/System.hpp>


namespace Hybrid
{ 
    namespace ECS
    {
        System::System():
            m_world(nullptr), m_enabled(true)
        {

        }

        System::~System()
        {

        }

        void System::setEnabled(bool enabled)
        {
            m_enabled = enabled;
        }

        bool System::isEnabled() const
        {
            return m_enabled;
        }

        bool System::toogle()
        {
            return (m_enabled = !m_enabled);
        }

        void System::onRegisteredWorld(World * world)
        {
            m_world = world;
            onRegistered();
        }

        void System::onRegistered()
        {

        }
    }
}
