#ifndef HYBRID_ECS_COMPONENT_MANAGER_HPP
#define HYBRID_ECS_COMPONENT_MANAGER_HPP

#include <memory>
#include <cassert>
#include <utility>

#include <Managers/Manager.hpp>
#include <Core/EntityInfo.hpp>
#include <Utils/Types.hpp>

namespace Hybrid
{ 
    namespace ECS
    {
        class ComponentManager : public Manager
        {
            public:
                ComponentManager();
                virtual ~ComponentManager();
                
                // Add a DefaultConstructible Component to the entity
                template <typename T>
                auto addComponent(EntityInfo & e) -> T *;

                // Construct and add a component of type T into the World
                template <typename T, typename ...Args>
                auto addComponent(EntityInfo & e, Args && ...args) -> T *;
            
                // Get the entity's component of type T
                //  /!\ Return nullptr if the entity does not have the component
                template <typename T>
                auto getComponent(EntityInfo const & e) -> T *;

                // Get all the component of type T
                template <typename T>
                auto getAllComponents() -> Vector<T> &;

                // Remove the entity's component of type T
                template <typename T>
                void removeComponent(EntityInfo & e);

                // Remove all the entity's components
                void removeAllComponents(EntityInfo & e) const;

                // Tell if the entity has a component of type T
                template <typename T>
                bool hasComponent(EntityInfo const & e);


                virtual void onRegistered() override;

            protected:
                template <typename T>
                auto getComponentContainer() -> Vector<T> &;


            private:
                using ComponentContainerPtr = std::shared_ptr<void>;
                Vector<ComponentContainerPtr> m_components;
        };

        #include "ComponentManager.inl"
    }
}

#endif //HYBRID_ECS_COMPONENT_MANAGER_HPP
