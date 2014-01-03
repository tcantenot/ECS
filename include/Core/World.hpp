#ifndef HYBRID_ECS_WORLD_HPP
#define HYBRID_ECS_WORLD_HPP

#include <utility>
#include <cassert>

#include <Managers/MetaManager.hpp>
#include <Managers/ComponentManager.hpp>
#include <Managers/EntityManager.hpp>
#include <Managers/GroupManager.hpp>
#include <Managers/SystemManager.hpp>
#include <Managers/VariableManager.hpp>

#include <Utils/Types.hpp>

#include <Exceptions/DeadEntityException.hpp>
#include <Exceptions/NotInitializedEntityException.hpp>

namespace Hybrid
{
    namespace ECS
    {
        class World
        {
            using Time = float;

            public:
                World();
                virtual ~World();

                World(World const &) = delete;
                World & operator=(World const &) = delete;

                // ---------------------------------------------------------------------
                //  SELF
                // ---------------------------------------------------------------------
                
                // Call this on each loop start
                void onLoopStart();

                // Set the elapsed time since the last update
                void setDelta(Time delta);

                // Get the elapsed time since the last update
                Time getDelta();

                // Update all the subsystems
                //  /!\ No specific order is guaranteed
                //  => if the order does matter, use the update() method of each system
                void update();
                
                // ---------------------------------------------------------------------
                //  ENTITY
                // ---------------------------------------------------------------------
                
                // Creates a new entity and stores it in the system
                Entity newEntity();
                
                // If(now) : the entity is immediately remove from the World
                // Else    : the entity is queuefor removal and actually removed
                //           on the next onLoopStart();
                void remove(Entity const & e, bool now = false)
                    throw(DeadEntityException, NotInitializedEntityException);

                // Tells if the entity is alive in the system
                bool isAlive(Entity const & e);

                // Get the info of an entity
                EntityInfo const & getEntityInfo(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException);

                // Number of entities in the World
                std::size_t count() const;

                // Total number of removed entities
                std::size_t totalCreated() const;

                // Total number of removed entities
                std::size_t totalRemoved() const;

                // ---------------------------------------------------------------------
                //  COMPONENT
                // ---------------------------------------------------------------------
         
                // Add a DefaultConstructible Component to the entity
                template <typename T>
                auto addComponent(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException)
                    -> T *;

                // Construct and add a component of type T into the World
                template <typename T, typename ...Args>
                auto addComponent(Entity const & e, Args && ...args)
                    throw(DeadEntityException, NotInitializedEntityException)
                    -> T *;

                // Get the entity's component of type T
                //  /!\ Return nullptr if the entity does not have the component
                //  /!\ The pointer can be invalidated when others component are
                //      added -> need to call getComponent again
                template <typename T>
                auto getComponent(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException)
                    -> T *;

                // Get all the component of type T
                template <typename T>
                auto getAllComponents() -> Vector<T> &;

                // Remove the entity's component of type T
                template <typename T>
                void removeComponent(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException);

                // Remove all the entity's components
                void removeAllComponents(Entity const & e) const
                    throw(DeadEntityException, NotInitializedEntityException);

                // Tell if the entity has a component of type T
                template <typename T>
                bool hasComponent(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException);

                // ---------------------------------------------------------------------
                //  MANAGER
                // ---------------------------------------------------------------------
         
                // Register a DefaultConstructible manager of type T into the World
                template <typename T>
                auto registerManager() -> ManagerPtr<T>;
                
                // Construct and add a manager of type T into the World
                template <typename T, typename ...Args>
                auto registerManager(Args && ...args) -> ManagerPtr<T>;

                // Register a manager of type T into the World
                template <typename T>
                auto registerManager(ManagerPtr<T> const & manager) -> ManagerPtr<T>;

                // Get the manager of type T of the World
                //  /!\ Return nullptr if the World does not have the manager
                template <typename T>
                auto getManager() const -> ManagerPtr<T>;

                //TODO: add removeManager() with static_assert 
                //      to check type different from the default manager

                // Tells if the World has a Manager of type T
                template <typename T>
                bool hasManager() const;

                // ---------------------------------------------------------------------
                //  SYSTEM
                // ---------------------------------------------------------------------
         
                // Register a DefaultConstructible system of type T into the World
                template <typename T>
                auto registerSystem() -> T *;

                // Construct and add a system of type T into the World
                template <typename T, typename ...Args>
                auto registerSystem(Args && ...args) -> T *;

                // Get the system of type T registered in the World
                //  /!\ Return nullptr if the World does not have the system
                template <typename T>
                auto getSystem() -> T *;

                // Remove the system of type T from the World
                template <typename T>
                void removeSystem();

                // Tell if the World has a system of type T
                template <typename T>
                bool hasSystem() const;

                // Update all the Systems
                void updateSystems();

                // Update all the EntitySystems
                void updateEntitySystems();

                // ---------------------------------------------------------------------
                //  GROUP
                // ---------------------------------------------------------------------
                
                // Add the entity to a group
                void addToGroup(Entity const & e, std::string const & group)
                    throw(DeadEntityException, NotInitializedEntityException);

                // Remove the entity from a group
                void removeFromGroup(Entity const & e, std::string const & group)
                    throw(DeadEntityException, NotInitializedEntityException);
                
                // Remove the entity from all its groups
                void removeFromAllGroups(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException);

                // Tells if the entity in is thee given group
                bool isInGroup(Entity const & e, std::string const & group)
                    throw(DeadEntityException, NotInitializedEntityException);

                // Retrieve all the entities in the given group
                Vector<Entity> const & getEntitiesByGroup(std::string const & group);

                // Get all the groups the entity is in
                //
                // /!\ Expensive call
                Vector<std::string> getGroupsByEntity(Entity const & e)
                    throw(DeadEntityException, NotInitializedEntityException);


                // ---------------------------------------------------------------------
                //  TAG
                // ---------------------------------------------------------------------

                // ---------------------------------------------------------------------
                //  VARIABLE
                // ---------------------------------------------------------------------
         
                template <typename T>
                void setValue(std::string const & name, T const & value);

                template <typename T>
                T const & getValue(std::string const & name);

               
            private:
                MetaManager      m_metaManager;
                ManagerPtr<EntityManager>     m_entityManager;
                ManagerPtr<ComponentManager>  m_componentManager;
                ManagerPtr<SystemManager>     m_systemManager;
                ManagerPtr<GroupManager>      m_groupManager;
                //ManagerPtr<TagManager>        m_tagManager;
                ManagerPtr<VariableManager>   m_variableManager;        
                
                Time m_delta;
        };

        #include "World.inl"
    }
}

#endif //HYBRID_ECS_WORLD_HPP
