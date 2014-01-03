#ifndef HYBRID_ECS_NOT_INITIALIZED_ENTITY_EXCEPTION
#define HYBRID_ECS_NOT_INITIALIZED_ENTITY_EXCEPTION

#include <exception>

namespace Hybrid
{
    namespace ECS
    {
        class NotInitializedEntityException : public std::exception
        {
            public:
                virtual char const * what() const throw();
        };
    }
}

#endif //HYBRID_ECS_NOT_INITIALIZED_ENTITY_EXCEPTION
