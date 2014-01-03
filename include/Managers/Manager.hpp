#ifndef HYBRID_ECS_MANAGER_HPP
#define HYBRID_ECS_MANAGER_HPP

namespace Hybrid
{
    namespace ECS
    { 
        class World;

        class Manager
        {
            public:
                Manager();
                virtual ~Manager();

                Manager(Manager const &) = delete;
                Manager & operator=(Manager const &) = delete;

                virtual void onRegistered() = 0;

            protected:
                friend class World;
                friend class MetaManager;

                World * m_world;
        };
    }
}

#endif //HYBRID_ECS_MANAGER_HPP
