#ifndef COLOR_HPP
#define COLOR_HPP

#include <HybridECS.hpp>

struct Color : public Hybrid::ECS::Component<Color>
{
    Color(): color(0) { }

    Color(int r, int g, int b, int a = 255):
        color(a + (b << 8) + (g << 16) + (r << 24)) { }

    unsigned int color;
};

#endif //COLOR_HPP
