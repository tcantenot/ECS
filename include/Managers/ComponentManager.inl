#include "ComponentManager.hpp"

template <typename T>
auto ComponentManager::addComponent(EntityInfo & e) -> T *
{
    static_assert(std::is_base_of<Component<T>, T>::value,
                  "T must be a derived class of Component");

    // Vector<T> &
    auto & components = this->getComponentContainer<T>();

    auto const id = e.id();

    if(id >= components.capacity())
    {
        components.reserve(2 * id + 1);
    }

    static_assert(std::is_default_constructible<T>::value,
                  "A component must be default constructible");

    auto size = std::max<std::size_t>(id + 1, components.size());
    components.resize(size);

    static_assert(std::is_copy_assignable<T>::value ||
                  std::is_move_assignable<T>::value,
                  "A component must be copy or move assignable");

    components[id] = T();

    e.addComponent(T::BitId);

    return &(components[id]);
}

template <typename T, typename ...Args>
auto ComponentManager::addComponent(EntityInfo & e, Args && ...args) -> T *
{
    static_assert(std::is_base_of<Component<T>, T>::value,
                  "T must be a derived class of Component");

    // Vector<T> &
    auto & components = this->getComponentContainer<T>();

    auto const id = e.id();

    if(id >= components.capacity())
    {
        components.reserve(2 * id + 1);
    }

    static_assert(std::is_default_constructible<T>::value,
                  "A Component must be default constructible");
    
    auto size = std::max<std::size_t>(id + 1, components.size());
    components.resize(size);

    static_assert(std::is_copy_assignable<T>::value ||
                  std::is_move_assignable<T>::value,
                  "A component must be copy or move assignable");

    components[id] = T(std::forward<Args>(args)...);

    e.addComponent(T::BitId);

    return &(components[id]);
}

template <typename T>
auto ComponentManager::getComponent(EntityInfo const & e) -> T *
{
    static_assert(std::is_base_of<Component<T>, T>::value,
                  "T must be a derived class of Component");

    #ifndef HYBRID_ECS_FAST
    if(hasComponent<T>(e))
    #endif
    {
        assert(T::BitId < m_components.size());

        auto const & components = 
            std::static_pointer_cast<Vector<T>>(m_components[T::BitId]);

        assert(components != nullptr);
        assert(e.id() < components->size());

        return static_cast<T *>(&((*components)[e.id()]));
    }

    return nullptr;
}

template <typename T>
auto ComponentManager::getAllComponents() -> Vector<T> &
{
    static_assert(std::is_base_of<Component<T>, T>::value,
                  "T must be a derived class of Component");

    return this->getComponentContainer<T>();
}

template <typename T>
void ComponentManager::removeComponent(EntityInfo & e)
{
    static_assert(std::is_base_of<Component<T>, T>::value,
                  "T must be a derived class of Component");

    // Clear the bit associated to the component
    e.removeComponent(T::BitId);
}

template <typename T>
bool ComponentManager::hasComponent(EntityInfo const & e)
{
    static_assert(std::is_base_of<Component<T>, T>::value,
                  "T must be a derived class of Component");

    return e.m_componentBitMask.test(T::BitId);
}

template <typename T>
auto ComponentManager::getComponentContainer() -> Vector<T> &
{
    static_assert(std::is_base_of<Component<T>, T>::value,
                  "T must be a derived class of Component");

    if(T::BitId >= m_components.capacity())
    {
        m_components.reserve(2 * T::BitId + 1);
    }

    if(T::BitId >= m_components.size())
    {
        auto size = std::max<std::size_t>(T::BitId + 1, m_components.size());
        m_components.resize(size);
    }

    if(m_components[T::BitId] == nullptr)
    {
        m_components[T::BitId] = std::make_shared<Vector<T>>();
        std::static_pointer_cast<Vector<T>>(m_components[T::BitId])->reserve(0x10);
    }

    return *(std::static_pointer_cast<Vector<T>>(m_components[T::BitId]).get());
}

