#ifndef HYBRID_ECS_SYSTEM_MANAGER_HPP
#define HYBRID_ECS_SYSTEM_MANAGER_HPP

#include <type_traits>
#include <utility>

#include <Managers/Manager.hpp>
#include <Managers/EntityManager.hpp>
#include <Core/EntityInfo.hpp>
#include <Systems/System.hpp>
#include <Systems/EntitySystem.hpp>
#include <Utils/Types.hpp>
#include <Utils/TypeIndexHash.hpp>

namespace Hybrid
{
    namespace ECS
    {
        // Make a distinction between the systems that inherit from
        // EntitySystem and those that only inherit from System by using
        // SFINAE principle.
        // 
        // Disclaimer : NotEntitySystemType could have been
        //
        //  template <typename T, typename R = void>
        //  typename std::enable_if<!std::is_base_of<EntitySystem<T>, T>::value
        //                          &&
        //                          std::is_base_of<System, T>::value, R>::type;
        //
        // to ensure that the T type is a child of System but I choose to use
        // a static_assert(std::is_base_of<System, T>::value, "") to deliver
        // a more understandable compilation error message.
        //
        template <typename T, typename R = void>
        using EntitySystemType = typename std::enable_if
            <std::is_base_of<EntitySystem<T>, T>::value, R>::type; 

        template <typename T, typename R = void>
        using NotEntitySystemType = typename std::enable_if
            <!std::is_base_of<EntitySystem<T>, T>::value, R>::type; 


        class SystemManager : public Manager
        {
            public:
                SystemManager();
                virtual ~SystemManager();

                // Register a DefaultConstructible system of type T
                // into the SystemManager
                template <typename T>
                auto registerSystem() -> EntitySystemType<T, T *>;

                template <typename T>
                auto registerSystem() -> NotEntitySystemType<T, T *>;

                // Construct and add a system of type T into the SystemManager
                template <typename T, typename ...Args>
                auto registerSystem(Args && ...args)
                    -> EntitySystemType<T, T *>;

                template <typename T, typename ...Args>
                auto registerSystem(Args && ...args)
                    -> NotEntitySystemType<T, T *>;

                // Get the system of type T registered in the SystemManager
                //  /!\ Return nullptr if the SystemManager 
                //      does not have the system
                template <typename T>
                auto getSystem() -> EntitySystemType<T, T *>;

                template <typename T>
                auto getSystem() -> NotEntitySystemType<T, T *>;

                // Remove the system of type T from the SystemManager
                template <typename T>
                auto removeSystem() -> EntitySystemType<T>;

                template <typename T>
                auto removeSystem() -> NotEntitySystemType<T>;

                // Tell if the SystemManager has a system of type T
                template <typename T>
                auto hasSystem() const -> EntitySystemType<T, bool>;

                template <typename T>
                auto hasSystem() const -> NotEntitySystemType<T, bool>;

                // Called when the World registered this manager
                virtual void onRegistered() override;

                // Called on loop start
                void onLoopStart();

                // Update all the systems (Systems and EntitySystems)
                void update();

                // Update all the Systems
                void updateSystems();
                
                // Update all the EntitySystems
                void updateEntitySystems();

            protected:
                friend class World;
                friend class EntityManager;
              
                void queueForUpdate(Entity const & e);
                void queueForUpdate(Entity const & e, EntityInfo & info);

                void updateEntity(Entity const & e);
                void updateEntity(Entity const & e, EntityInfo & info);

                // Add already existing entities to the new registered system
                template <typename T>
                void addEntitiesToNewSystem(T * system);
                
                void remove(EntityInfo & info);

            private:
                template <typename T>
                void checkEntitySystemContainer();


            private:
                using SystemPtr = std::shared_ptr<System>;
                using EntitySystemPtr = std::shared_ptr<BaseEntitySystem>;

                Hash<TypeIndexHash, SystemPtr> m_systems;
                Vector<EntitySystemPtr> m_entitySystems;
                Vector<Entity> m_updatedEntities;
                ManagerPtr<EntityManager> m_entityManager;
        };

        #include "SystemManager.inl"
    }
}

#endif //HYBRID_ECS_SYSTEM_MANAGER_HPP
