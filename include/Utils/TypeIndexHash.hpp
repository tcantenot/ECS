#ifndef HYBRID_ECS_TYPE_INDEX_HASH_HPP
#define HYBRID_ECS_TYPE_INDEX_HASH_HPP

#include <typeindex>
#include <functional>

namespace Hybrid
{
    namespace ECS
    {
        struct TypeIndexHash
        {
            TypeIndexHash(std::type_index const & type):
                m_hash(std::hash<std::type_index>()(type))
            {

            }

            std::size_t const m_hash;
        };
    }
}

namespace std
{
    template <>
    struct hash<Hybrid::ECS::TypeIndexHash>
    {
        std::size_t operator()(Hybrid::ECS::TypeIndexHash const & type) const
        {
            return type.m_hash;
        }
    };

    template <>
    struct equal_to<Hybrid::ECS::TypeIndexHash>
    {
        std::size_t operator()(Hybrid::ECS::TypeIndexHash const & lhs, 
                               Hybrid::ECS::TypeIndexHash const & rhs) const
        {
            return lhs.m_hash == rhs.m_hash;
        }
    };
}
    
#endif //HYBRID_ECS_TYPE_INDEX_HASH_HPP
