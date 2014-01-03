#ifndef VELOCITY_HPP
#define VELOCITY_HPP

#include <HybridECS.hpp>

struct Velocity : public Hybrid::ECS::Component<Velocity>
{
    Velocity(float x = 0, float y = 0): x(x), y(y) { }

    float x, y;
};


#endif //VELOCITY_HPP
