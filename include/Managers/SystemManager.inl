#include "SystemManager.hpp"


namespace
{
    template <typename T>
    inline T * static_ptr_cast(std::shared_ptr<void> const & ptr)
    {
        return (std::static_pointer_cast<T>(ptr)).get();
    }
}


template <typename T>
auto SystemManager::registerSystem() -> EntitySystemType<T, T *>
{
    checkEntitySystemContainer<T>();

    m_entitySystems[T::BitId] = std::make_shared<T>();

    auto system = static_ptr_cast<T>(m_entitySystems[T::BitId]);

    system->onRegisteredWorld(m_world);

    system->registerComponents();

    this->addEntitiesToNewSystem(system);

    return system;
}


template <typename T>
auto SystemManager::registerSystem() -> NotEntitySystemType<T, T *>
{
    static_assert(std::is_base_of<System, T>::value,
                  "T must be a derived class of System");

    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });

    auto const & p = m_systems.emplace(index, std::make_shared<T>());

    auto system = std::static_pointer_cast<T>(p.first->second).get();
        
    system->onRegisteredWorld(m_world);

    return system;
}


template <typename T, typename ...Args>
auto SystemManager::registerSystem(Args && ...args)
    -> EntitySystemType<T, T *>
{
    checkEntitySystemContainer<T>();

    m_entitySystems[T::BitId] =
        std::make_shared<T>(std::forward<Args>(args)...);

    auto system = static_ptr_cast<T>(m_entitySystems[T::BitId]);

    system->onRegisteredWorld(m_world);

    system->registerComponents();

    this->addEntitiesToNewSystem(system);

    return system;
}

template <typename T, typename ...Args>
auto SystemManager::registerSystem(Args && ...args) 
    -> NotEntitySystemType<T, T *>
{
    static_assert(std::is_base_of<System, T>::value,
                  "T must be a derived class of System");

    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });

    auto const & p = m_systems.emplace(index, 
                        std::make_shared<T>(std::forward<Args>(args)...));

    auto system = std::static_pointer_cast<T>(p.first->second).get();
        
    system->onRegisteredWorld(m_world);

    return system;
}


template <typename T>
auto SystemManager::getSystem() -> EntitySystemType<T, T *>
{
    //TODO: Performances -> replace by assert
    if(T::BitId < m_entitySystems.size())
    {
        if(m_entitySystems[T::BitId])
        {
            return static_ptr_cast<T>(m_entitySystems[T::BitId]);
        }
    }

    return nullptr;        
}


template <typename T>
auto SystemManager::getSystem() -> NotEntitySystemType<T, T *>
{
    static_assert(std::is_base_of<System, T>::value,
                  "T must be a derived class of System");

    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });

    auto const it = m_systems.find(index);

    if(it != m_systems.end())
    {
        return std::static_pointer_cast<T>(it->second).get();
    }

    return nullptr; 
}


template <typename T>
auto SystemManager::removeSystem() -> EntitySystemType<T>
{
    //TODO: Performances -> replace by assert
    if(T::BitId < m_entitySystems.size())
    {
        assert(m_entitySystems[T::BitId] != nullptr);

        auto system = static_ptr_cast<T>(m_entitySystems[T::BitId]);

        for(auto & e : system->m_entities)
        {
            auto & info = m_entityManager->getEntityInfo(e);
            info.removeSystem(T::BitId);
        }

        system->m_entities.clear();
        //TODO : create a method reset();

        for(auto & e : m_updatedEntities)
        {
            auto & info = m_entityManager->getEntityInfo(e);
            info.removeSystem(T::BitId);
        }

        m_entitySystems[T::BitId].reset();
        m_entitySystems[T::BitId] == nullptr;
    }
}

template <typename T>
auto SystemManager::removeSystem() -> NotEntitySystemType<T>
{
    static_assert(std::is_base_of<System, T>::value,
                  "T must be a derived class of System");

    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });

    auto const it = m_systems.find(index);

    if(it != m_systems.end())
    {
        m_systems.erase(it);
    }
}

template <typename T>
auto SystemManager::hasSystem() const -> EntitySystemType<T, bool>
{
    //TODO: Performances -> replace by assert
    if(T::BitId < m_entitySystems.size())
    {
        return m_entitySystems[T::BitId] != nullptr;
    }

    return false;
}


template <typename T>
auto SystemManager::hasSystem() const -> NotEntitySystemType<T, bool>
{
    static_assert(std::is_base_of<System, T>::value,
                  "T must be a derived class of System");

    static auto const index = TypeIndexHash(std::type_index{ typeid(T) });
    return m_systems.find(index) != m_systems.end();
}


template <typename T>
void SystemManager::addEntitiesToNewSystem(T * system)
{
    for(auto & info : m_entityManager->m_entityInfos)
    {
        if(info.matchesComponentBitMask(system->componentBitMask()))
        {
            assert(!info.matchesSystemBit(system->bitId()));

            system->addEntity(Entity(info), info);
        }
    }
}

template <typename T>
void SystemManager::checkEntitySystemContainer()
{
    if(T::BitId >= m_entitySystems.capacity())
    {
        m_entitySystems.reserve(2 * T::BitId + 1);
    }

    if(T::BitId >= m_entitySystems.size())
    {
        auto size = std::max<std::size_t>(T::BitId + 1, m_entitySystems.size());
        m_entitySystems.resize(size);
    }
}
