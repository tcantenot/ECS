#ifndef HYBRID_ECS_COMPONENT_HPP
#define HYBRID_ECS_COMPONENT_HPP

#include <Utils/Types.hpp>

namespace Hybrid
{
    namespace ECS
    { 
        struct BaseComponent
        {
            BaseComponent() = default;
            virtual ~BaseComponent() { }

            protected:
                static ComponentBit s_nextBitId;
        };

        template <typename T>
        struct Component : private BaseComponent
        {
            Component() = default;
            virtual ~Component() { }

            static ComponentBit const BitId;
        };

        template <typename T>
        ComponentBit const Component<T>::BitId = BaseComponent::s_nextBitId++;

        // How to use it :
        // struct Position : public Component<Position> { };
    }
}

#endif //HYBRID_ECS_COMPONENT_HPP
