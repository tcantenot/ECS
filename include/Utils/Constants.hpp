#ifndef HYBRID_ECS_CONSTANTS_HPP
#define HYBRID_ECS_CONSTANTS_HPP

#include <limits>
#include <cstdint>

namespace Hybrid
{ 
    namespace ECS
    {
        namespace Constants
        {
            static auto const MAX_ENTITIES               = std::numeric_limits<uint32_t>::max(); 
            static auto const MAX_COMPONENTS             = uint32_t(64);
            static auto const MAX_ENTITY_SYSTEMS         = uint32_t(64);
            static auto const MAX_GROUPS                 = uint32_t(64);
            static auto const ENTITY_UNDEFINED_UNIQUE_ID = uint32_t(0);
            static auto const ENTITY_FIRST_UNIQUE_ID     = uint32_t(1);

            inline void remove_unused_warnings()
            {
                (void) MAX_ENTITIES; 
                (void) MAX_COMPONENTS;
                (void) MAX_ENTITY_SYSTEMS;
                (void) MAX_GROUPS;
                (void) ENTITY_UNDEFINED_UNIQUE_ID;
                (void) ENTITY_FIRST_UNIQUE_ID;
            }

        }
    }
}

#endif //HYBRID_ECS_CONSTANTS_HPP
