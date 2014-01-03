#include <Exceptions/DeadEntityException.hpp>

#include <sstream>

namespace Hybrid
{ 
    namespace ECS
    {
        DeadEntityException::DeadEntityException(Entity const & e): m_entity(e)
        {

        }


        char const * DeadEntityException::what() const throw()
        {
            std::ostringstream oss; 

            oss << "The entity " << m_entity << " is dead.";

            return oss.str().c_str();
        }
    }
}
