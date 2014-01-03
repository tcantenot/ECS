#include <Exceptions/NotInitializedEntityException.hpp>


namespace Hybrid
{ 
    namespace ECS
    {
        char const * NotInitializedEntityException::what() const throw()
        {
            return "The entity is not initialized.";
        }
    }
}
