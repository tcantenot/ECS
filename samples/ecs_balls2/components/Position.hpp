#ifndef POSITION_HPP
#define POSITION_HPP

#include <HybridECS.hpp>

struct Position : public Hybrid::ECS::Component<Position>
{
    Position(float x = 0, float y = 0): x(x), y(y) { }

    float x, y;
};

#endif //POSITION_HPP
