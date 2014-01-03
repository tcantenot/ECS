#ifndef BALLS_HPP
#define BALLS_HPP

#include <memory>

#include "Color.hpp" 

struct Ball
{
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

using BallPtr = Ball *;//std::shared_ptr<Ball>;

#endif //BALLS_HPP
