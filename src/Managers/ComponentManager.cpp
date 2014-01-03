#include <Managers/ComponentManager.hpp>
#include <Core/World.hpp>

namespace Hybrid
{ 
    namespace ECS
    {
        ComponentManager::ComponentManager() : 
            Manager(), m_components()
        {

        }

        ComponentManager::~ComponentManager()
        {

        }

        void ComponentManager::onRegistered()
        {
            assert(m_world != nullptr);
        }

        void ComponentManager::removeAllComponents(EntityInfo & e) const
        {
            e.removeAllComponents();
        }
    }
}
