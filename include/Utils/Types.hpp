#ifndef HYBRID_ECS_TYPES_HPP
#define HYBRID_ECS_TYPES_HPP

#include <bitset>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Utils/Constants.hpp>

namespace Hybrid
{
    namespace ECS
    {
        class Entity;
        template <typename T> struct Component;
        class Manager;
        class System;
        template <typename T> class EntitySystem;


        // ---------------------------------------------------------------------
        //  Template types
        // ---------------------------------------------------------------------
        template <typename T>
        using SmartPtr = std::shared_ptr<T>;

        template <typename Key, typename Value>
        using Hash = std::unordered_map<Key, Value>;

        template <typename Key>
        using Set = std::unordered_set<Key>;

        template <typename T>
        using Vector = std::vector<T>;

        template <int N>
        using BitMask = std::bitset<N>;


        // ---------------------------------------------------------------------
        //  Global aliases
        // ---------------------------------------------------------------------
        using EntityId        = uint32_t;
        using ComponentBit    = uint32_t;
        using EntitySystemBit = uint32_t;
        using GroupBit        = uint32_t;
        using Index           = uint32_t;

        using EntitySystemBitMask = BitMask<Constants::MAX_ENTITY_SYSTEMS>;

        using ComponentBitMask    = BitMask<Constants::MAX_COMPONENTS>;

        using GroupBitMask        = BitMask<Constants::MAX_GROUPS>;

        // ---------------------------------------------------------------------
        //  MetaManager types
        // ---------------------------------------------------------------------
        template <typename T>
        using ManagerPtr = SmartPtr<T>;

        // ---------------------------------------------------------------------
        //  ComponentManager types
        // ---------------------------------------------------------------------
        template <typename T>
        using ComponentPtr = SmartPtr<T>;

        template <typename T>
        using ComponentPtrContainer = Vector<ComponentPtr<Component<T>>>;

        template <typename T>
        using ComponentPtrContainerPtr = SmartPtr<ComponentPtrContainer<T>>;

        template <typename T>
        using ComponentContainer = Vector<T>;

        // ---------------------------------------------------------------------
        //  SystemManager types
        // ---------------------------------------------------------------------
        template <typename T>
        using SystemPtr = SmartPtr<T>;

        using SystemPtrContainer = Vector<SystemPtr<System>>;

        template <typename T>
        using EntitySystemPtrContainer = Vector<SystemPtr<EntitySystem<T>>>;
    }
}

#endif //HYBRID_ECS_TYPES_HPP
