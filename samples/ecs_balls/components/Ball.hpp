#ifndef BALLS_HPP
#define BALLS_HPP

#include <HybridECS.hpp>

#include "Color.hpp" 

struct Ball : public Hybrid::ECS::Component<Ball>
{
    Ball() = default;
    Ball(float x, float y, float vx ,float vy, float r, Color c)
        :x(x), y(y), vx(vx), vy(vy), r(r), c(c)
    {

    }

    float x;
    float y;
    float vx;
    float vy;
    float r;
    Color c;
};

#endif //BALLS_HPP
