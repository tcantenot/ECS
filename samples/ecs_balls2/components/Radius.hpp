#ifndef RADIUS_HPP
#define RADIUS_HPP

#include <HybridECS.hpp>

struct Radius : public Hybrid::ECS::Component<Radius>
{
    Radius(float radius = 0): radius(radius) { }

    float radius;
};

#endif //RADIUS_HPP
