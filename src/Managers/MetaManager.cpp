#include <Managers/MetaManager.hpp>


namespace Hybrid
{ 
    namespace ECS
    {
        MetaManager::MetaManager(World * world):
            m_world(world), m_managers()
        {

        }

        MetaManager::~MetaManager()
        {

        }
    }
}
