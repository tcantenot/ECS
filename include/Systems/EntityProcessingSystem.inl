#include "EntityProcessingSystem.hpp"

template <typename T>
EntityProcessingSystem<T>::EntityProcessingSystem(): EntitySystem<T>()
{

}

template <typename T>
EntityProcessingSystem<T>::~EntityProcessingSystem()
{

}

template <typename T>
void EntityProcessingSystem<T>::processEntities(Vector<Entity> & entities)
{
    for(auto const & e : entities)
    {
        process(e);
    }
}
