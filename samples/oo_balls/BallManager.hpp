#ifndef BALL_MANAGER_HPP
#define BALL_MANAGER_HPP

#include "Ball.hpp"

#include <vector>
#include <cstdlib>

class BallManager
{
    public:
        BallManager(int w, int h);
        ~BallManager();

        void setDimensions(int w, int h);

        void createBalls(int nb);
        void destroyBalls(int nb);

        int ballCount() const;

        std::vector<Ball> & balls();

    private:
        int m_w;
        int m_h;


        static const int MIN_RADIUS = 2;
        static const int MAX_RADIUS = 3;

        static const int MIN_INITIAL_VELOCITY_X = 100;
        static const int MAX_INITIAL_VELOCITY_X = 300;
        static const int MIN_INITIAL_VELOCITY_Y = 100;
        static const int MAX_INITIAL_VELOCITY_Y = 300;

        std::vector<Ball> m_balls;
};

BallManager::BallManager(int w, int h)
    : m_w(w), m_h(h)
{

}

BallManager::~BallManager()
{
    m_balls.clear();
}

void BallManager::createBalls(int nb)
{
    for (int i = 0; i < nb; ++i)
    {
        // At a random position on the screen
        float x = (float)(rand() % m_w);
        float y = (float)(rand() % m_h);

        // With a random velocity between (10, 10) and (100, 100)
        float vx = (float)(rand() % (MAX_INITIAL_VELOCITY_X - MIN_INITIAL_VELOCITY_X) + MIN_INITIAL_VELOCITY_X);
        float vy = (float)(rand() % (MAX_INITIAL_VELOCITY_Y - MIN_INITIAL_VELOCITY_Y) + MIN_INITIAL_VELOCITY_Y);

        // A random radius between 10 and 50
        float r = (float)(rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS);

        // And a random colour
        Color c(rand() % 255, rand() % 255, rand() % 255, 255);

        // Add entity to bag
        m_balls.emplace_back(x, y, vx, vy, r, c);
    }
}

void BallManager::destroyBalls(int nb)
{
    for (int i = 0; i < nb; ++i)
    {
        if (m_balls.size() > 0)
        {
            m_balls.pop_back();
        }
    }
}

int BallManager::ballCount() const
{
    return m_balls.size();
}

std::vector<Ball> & BallManager::balls()
{
    return m_balls;
}

#endif //BALL_MANAGER_HPP
