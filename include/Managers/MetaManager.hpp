#ifndef HYBRID_ECS_META_MANAGER_HPP
#define HYBRID_ECS_META_MANAGER_HPP

#include <memory>
#include <unordered_map>

#include <Managers/Manager.hpp>
#include <Utils/Types.hpp>
#include <Utils/TypeIndexHash.hpp>

namespace Hybrid
{
    namespace ECS
    {
        class World;

        class MetaManager
        {
            public:
                MetaManager(World * world);
                virtual ~MetaManager();

                MetaManager(MetaManager const &) = delete;
                MetaManager & operator=(MetaManager const &) = delete;

                // Register a DefaultConstructible manager of type T into the Entity System
                template <typename T>
                auto registerManager() -> ManagerPtr<T>;
                
                // Register a manager of type T into the MetaManager
                template <typename T>
                auto registerManager(ManagerPtr<T> const & manager) -> ManagerPtr<T>;

                // Get the manager of type T of the MetaManager
                //  /!\ Return nullptr if the MetaManager does not have the manager
                template <typename T>
                auto getManager() const -> ManagerPtr<T>;

                // Tells if the MetaManager has a Manager of type T
                template <typename T>
                bool hasManager() const;

            private:
                World * m_world;
                Hash<TypeIndexHash, ManagerPtr<Manager>> m_managers;
        };

        #include "MetaManager.inl"
    }
}

#endif //HYBRID_ECS_META_MANAGER_HPP
