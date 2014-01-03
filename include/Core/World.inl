#include "World.hpp"

// ---------------------------------------------------------------------
//  COMPONENT
// ---------------------------------------------------------------------

template <typename T>
auto World::addComponent(Entity const & e)
    throw(DeadEntityException, NotInitializedEntityException)
    -> T *
{
    auto & info = m_entityManager->getEntityInfo(e);

    auto const & c = m_componentManager->addComponent<T>(info);

    m_systemManager->queueForUpdate(e, info);

    return c;
}

template <typename T, typename ...Args>
auto World::addComponent(Entity const & e, Args && ...args)
    throw(DeadEntityException, NotInitializedEntityException)
    -> T *
{
    auto & info = m_entityManager->getEntityInfo(e);

    auto const & c = m_componentManager->addComponent<T>(info, std::forward<Args>(args)...);

    m_systemManager->queueForUpdate(e, info);

    return c;
}


template <typename T>
auto World::getComponent(Entity const & e)
    throw(DeadEntityException, NotInitializedEntityException)
    -> T *
{
    auto & info = m_entityManager->getEntityInfo(e);
    return m_componentManager->getComponent<T>(info);
}

template <typename T>
auto World::getAllComponents() -> Vector<T> &
{
    return m_componentManager->getAllComponents<T>();
}

template <typename T>
void World::removeComponent(Entity const & e)
    throw(DeadEntityException, NotInitializedEntityException)
{
    auto & info = m_entityManager->getEntityInfo(e);

    m_componentManager->removeComponent<T>(info);

    // Remove immediately the entity from the non-matching systems
    m_systemManager->updateEntity(e, info);
}

template <typename T>
bool World::hasComponent(Entity const & e)
    throw(DeadEntityException, NotInitializedEntityException)
{
    auto & info = m_entityManager->getEntityInfo(e);
    return m_componentManager->hasComponent<T>(info);
}

// ---------------------------------------------------------------------
//  MANAGER
// ---------------------------------------------------------------------

template <typename T>
auto World::registerManager() -> ManagerPtr<T>
{
    return m_metaManager.registerManager<T>();
}

template <typename T, typename ...Args>
auto World::registerManager(Args && ...args) -> ManagerPtr<T>
{
    return m_metaManager.registerManager<T>(
        ManagerPtr<T>{ new T(std::forward<Args>(args)...) });
}

template <typename T>
auto World::registerManager(ManagerPtr<T> const & manager) -> ManagerPtr<T>
{
    return m_metaManager.registerManager<T>(manager);
}

template <typename T>
auto World::getManager() const -> ManagerPtr<T>
{
    return m_metaManager.getManager<T>();
}

template <typename T>
bool World::hasManager() const
{
    return m_metaManager.hasManager<T>();
}



// ---------------------------------------------------------------------
//  SYSTEM
// ---------------------------------------------------------------------

template <typename T>
auto World::registerSystem() -> T *
{
    return m_systemManager->registerSystem<T>();
}

template <typename T, typename ...Args>
auto World::registerSystem(Args && ...args) -> T *
{
    return m_systemManager->registerSystem<T>(std::forward<Args>(args)...);
}

template <typename T>
auto World::getSystem() -> T *
{
    return m_systemManager->getSystem<T>();
}

template <typename T>
void World::removeSystem()
{
    return m_systemManager->removeSystem<T>();
}

template <typename T>
bool World::hasSystem() const
{
    return m_systemManager->hasSystem<T>();
}

// ---------------------------------------------------------------------
//  GROUP
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
//  TAG
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
//  VARIABLE
// ---------------------------------------------------------------------

template <typename T>
void World::setValue(std::string const & name, T const & value)
{
    m_variableManager->setValue<T>(name, value);
}

template <typename T>
T const & World::getValue(std::string const & name)
{
    return m_variableManager->getValue<T>(name);
}

