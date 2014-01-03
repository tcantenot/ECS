#include "MetaManager.hpp"


template <typename T>
auto MetaManager::registerManager() -> ManagerPtr<T>
{
    return this->registerManager<T>(ManagerPtr<T>{ new T() });
}


template <typename T>
auto MetaManager::registerManager(ManagerPtr<T> const & manager) -> ManagerPtr<T>
{
    static_assert(std::is_base_of<Manager, T>::value,
                  "T must be a derived class of Manager");

    manager->m_world = m_world;

    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });

    m_managers[index] = manager;

    manager->onRegistered();
    
    return manager;
}


template <typename T>
auto MetaManager::getManager() const -> ManagerPtr<T>
{
    static_assert(std::is_base_of<Manager, T>::value,
                  "T must be a derived class of Manager");

    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });

    auto const it = m_managers.find(index);
    if(it != m_managers.end())
    {
        return std::static_pointer_cast<T>(it->second);
    }

    return ManagerPtr<T>(nullptr);
}

template <typename T>
bool MetaManager::hasManager() const
{
    static_assert(std::is_base_of<Manager, T>::value,
                  "T must be a derived class of Manager");

    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });

    return m_managers.find(index) != m_managers.end();
}


