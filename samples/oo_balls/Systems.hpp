#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "graphics/PixelBuffer.h"
#include "Ball.hpp"

#include <vector>

inline void process_collisions(int w, int h, std::vector<Ball> & balls)
{
    const float DAMPENING_VERTICAL   = 1; //0.95f;
    const float DAMPENING_HORIZONTAL = 1; //0.95f;

    for(auto & b : balls)
    {
        // If we hit a wall, bounce and apply a dampening coefficient
        if ((b.x - b.r < 0 && b.x < 0) || (b.x + b.r > w && b.x > 0))
        {
            b.vx = -b.vx * DAMPENING_HORIZONTAL;
        }

        if ((b.y - b.r < 0 && b.y < 0) || (b.y + b.r > h && b.y > 0))
        {
            b.vy = -b.vy * DAMPENING_VERTICAL;
        }
    }
}

inline void process_gravity(float dt, std::vector<Ball> & balls)
{
    const float GRAVITY_ACCELERATION = 500;

    for(auto & b : balls)
    {
        b.vy += dt * GRAVITY_ACCELERATION;
    }
}

inline void process_movement(float dt, std::vector<Ball> & balls)
{
    for(auto & b : balls)
    {
        // Update position
        b.x += b.vx * dt;
        b.y += b.vy * dt;
    }
}

inline void process_rendering(PixelBuffer * target, std::vector<Ball> & balls)
{
    for(auto const & b : balls)
    {
        // Draw circle with properties
        target->drawCircle(b.c.color, b.r, b.x, b.y);
    }
}


#endif //SYSTEMS_HPP
