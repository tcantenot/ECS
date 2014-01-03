#ifndef HYBRID_ECS_DEAD_ENTITY_EXCEPTION_HPP
#define HYBRID_ECS_DEAD_ENTITY_EXCEPTION_HPP

#include <exception>

#include <Core/Entity.hpp>

namespace Hybrid
{
    namespace ECS
    {
        class DeadEntityException : public std::exception
        {
            public:
                DeadEntityException(Entity const & e);

                virtual char const * what() const throw();

            private:
                Entity m_entity;
        };
    }
}

#endif //HYBRID_ECS_DEAD_ENTITY_EXCEPTION_HPP
