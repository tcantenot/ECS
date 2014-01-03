#ifndef BALL_MANAGER_HPP
#define BALL_MANAGER_HPP

#include <HybridECS.hpp>

#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Radius.hpp"
#include "../components/Color.hpp"

#include <vector>

class BallManager : public Hybrid::ECS::Manager
{
    public:
        BallManager(int w, int h);

        void setDimensions(int w, int h);

        void createBalls(int nb);
        void destroyBalls(int nb);

        int ballCount() const;

        virtual void onRegistered() override { }

    private:
        int m_w;
        int m_h;


        static const int MIN_RADIUS = 2;
        static const int MAX_RADIUS = 3;

        static const int MIN_INITIAL_VELOCITY_X = 100;
        static const int MAX_INITIAL_VELOCITY_X = 300;
        static const int MIN_INITIAL_VELOCITY_Y = 100;
        static const int MAX_INITIAL_VELOCITY_Y = 300;

        std::vector<Hybrid::ECS::Entity> m_balls;
};

BallManager::BallManager(int w, int h)
    : m_w(w), m_h(h)
{

}

void BallManager::createBalls(int nb)
{
    for (int i = 0; i < nb; ++i)
    {
        // Create a ball
        auto e = m_world->newEntity();

        // At a random position on the screen
        auto pos = m_world->addComponent<Position>(e);
        //pos->x = (float)(rand() % m_w);
        //pos->y = (float)(rand() % m_h);
        
        pos->x = static_cast<float>(m_w) / 2.f;
        pos->y = static_cast<float>(m_h) / 2.f;

        // With a random velocity between (10, 10) and (100, 100)
        auto vel = m_world->addComponent<Velocity>(e);
        auto coeffX = (float) (rand() % (100 - 0) + 0);
        auto coeffY = (float) (rand() % (100 - 0) + 0);
        vel->x = (coeffX > 50 ? 1 : -1) * (float)(rand() % (MAX_INITIAL_VELOCITY_X - MIN_INITIAL_VELOCITY_X) + MIN_INITIAL_VELOCITY_X);
        vel->y = (coeffY > 50 ? 1 : -1) * (float)(rand() % (MAX_INITIAL_VELOCITY_Y - MIN_INITIAL_VELOCITY_Y) + MIN_INITIAL_VELOCITY_Y);

        // A random radius between 10 and 50
        auto radius = m_world->addComponent<Radius>(e);
        radius->radius = (float)(rand() % (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS);

        // And a random colour
        m_world->addComponent<Color>(e, rand() % 255, rand() % 255, rand() % 255, 255);

        // Add entity to bag
        m_balls.push_back(e);
    }
}

void BallManager::destroyBalls(int nb)
{
    for (int i = 0; i < nb; ++i)
    {
        if (m_balls.size() > 0)
        {
            m_world->remove(m_balls.back());
            m_balls.pop_back();
        }
    }
}

int BallManager::ballCount() const
{
    return m_balls.size();
}


#endif //BALL_MANAGER_HPP
