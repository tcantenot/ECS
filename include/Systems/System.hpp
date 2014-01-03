#ifndef HYBRID_ECS_SYSTEM_HPP
#define HYBRID_ECS_SYSTEM_HPP

#include <Core/EntityInfo.hpp>
#include <Utils/Types.hpp>

namespace Hybrid
{ 
    namespace ECS
    { 
        class World;

        class System
        {
            public:
                System();
                virtual ~System();

                System(System const &) = delete;
                System & operator=(System const &) = delete;

                void onRegisteredWorld(World * world);

                virtual void update() = 0;

                void setEnabled(bool enabled);
                bool isEnabled() const;

                // Invert enabled state
                bool toogle();

            protected:
                friend class SystemManager;

                // Called when registered in the World
                //  -> onRegistered(World * world);
                virtual void onRegistered();

            protected:
                World * m_world;

            private:
                bool m_enabled;
        };
    }
}

#endif //HYBRID_ECS_SYSTEM_HPP
