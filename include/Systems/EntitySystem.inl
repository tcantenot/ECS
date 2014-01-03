#include "EntitySystem.hpp"


template <typename C>
void BaseEntitySystem::registerComponent()
{
    static_assert(std::is_base_of<Component<C>, C>::value,
                  "T must be a derived class of Component");

    m_componentBitMask.set(C::BitId);
}


template <typename T>
EntitySystemBit const EntitySystem<T>::BitId = BaseEntitySystemId::s_nextBitId++;

template <typename T>
EntitySystem<T>::EntitySystem():
    BaseEntitySystem(), BaseEntitySystemId()
{
    assert(T::BitId < Constants::MAX_ENTITY_SYSTEMS);
}

template <typename T>
EntitySystem<T>::~EntitySystem()
{

}


template <typename T>
EntitySystemBit const & EntitySystem<T>::bitId() const
{
    return T::BitId;
}


template <typename T>
void EntitySystem<T>::addEntity(Entity const & e, EntityInfo & info)
{
    if(!info.systemBitMask().test(T::BitId))
    {
        m_entities.push_back(e);
        m_lookup.insert(std::make_pair(e.id(), m_entities.size() - 1));
        info.addSystem(T::BitId);
        onEntityAdded(e);
    }
}


template <typename T>
void EntitySystem<T>::removeEntity(Entity const & e, EntityInfo & info)
{

    auto it = m_lookup.find(e.id());

    if(it != m_lookup.end())
    {
        auto index = it->second;
        assert(index < m_entities.size());

        m_entities[index] = m_entities[m_entities.size() - 1];        
        m_entities.pop_back();

        auto moved = m_lookup.find(m_entities[index].id());
        assert(moved != m_lookup.end());

        moved->second = index;

        m_lookup.erase(it);
    }

    info.removeSystem(T::BitId);
    onEntityRemoved(e);
}

