#include "SystemTests.hpp"

#include <HybridECS.hpp>

using namespace Hybrid::ECS;

SystemTests::SystemTests(): Test("System Tests")
{

}

namespace
{
    struct Position : public Component<Position>
    {
        Position(int x = 0, int y = 0): x(x), y(y) { }
        int x, y;
    };

    struct Velocity : public Component<Velocity>
    {
        Velocity(int x = 0, int y = 0): x(x), y(y) { }

        int x, y;
    };

    template <typename T>
    class TestEntitySystem : public EntityProcessingSystem<T>
    {
        public:
            TestEntitySystem():
                EntityProcessingSystem<T>(),
                m_registerComponentsCalled(false),
                m_onRegisteredCalled(false),
                m_onEntityAddedCalled(false),
                m_onEntityRemovedCalled(false),
                m_onPreProcessingCalled(false),
                m_onPostProcessingCalled(false),
                m_lastAddedEntity(42424242),
                m_lastRemovedEntity(42424242)
            {

            }

            virtual ~TestEntitySystem() { }

        protected:
            virtual void onRegisteredEnd() override
            {
                m_onRegisteredCalled = true;
            }


            virtual void registerComponents() override
            {
                m_registerComponentsCalled = true;
            }

            virtual void onEntityAdded(Entity const & e) override
            {
                m_onEntityAddedCalled = true;
                m_lastAddedEntity = e.uniqueId();
            }

            virtual void onEntityRemoved(Entity const & e) override
            {
                m_onEntityRemovedCalled = true;
                m_lastRemovedEntity = e.uniqueId();
            }

            virtual void onPreProcessing() override
            {
                m_onPreProcessingCalled = true;
            }

            virtual void onPostProcessing() override
            {
                m_onPostProcessingCalled = true;
            }

        public:
            bool m_registerComponentsCalled;
            bool m_onRegisteredCalled;
            bool m_onEntityAddedCalled;
            bool m_onEntityRemovedCalled;
            bool m_onPreProcessingCalled;
            bool m_onPostProcessingCalled;
            EntityId m_lastAddedEntity;
            EntityId m_lastRemovedEntity;
    };

    class MovementSystem : public TestEntitySystem<MovementSystem>
    {
        public:
            MovementSystem(): TestEntitySystem()
            {

            }

            virtual void process(Entity const & e)
            {
                auto p = m_world->getComponent<Position>(e);
                auto v = m_world->getComponent<Velocity>(e);

                p->x += v->x * static_cast<int>(m_world->getDelta());
                p->y += v->y * static_cast<int>(m_world->getDelta());
            }

            virtual void registerComponents() override
            {
                registerComponent<Position>();
                registerComponent<Velocity>();
                m_registerComponentsCalled = true;
            }

    };

    class EntitySystemWithParams : public TestEntitySystem<EntitySystemWithParams>
    {
        public:
            EntitySystemWithParams(int a, float b): TestEntitySystem(), m_a(a), m_b(b)
            {

            }

            virtual void process(Entity const &) { }

            virtual void registerComponents() override
            {
                registerComponent<Position>();
                registerComponent<Velocity>();
                m_registerComponentsCalled = true;
            }

            int m_a;
            float m_b;
    };


    class TestSystem : public System
    {
        public:
            TestSystem(): System()
            {

            }

            void update() { }
    };

    class TestSystemWithParams : public System
    {
        public:
            TestSystemWithParams(int a, float b): System(), m_a(a), m_b(b)
            {

            }

            void update() { }

            int m_a;
            float m_b;
    };


    struct Foo : public Component<Foo>
    {
        Foo() : Component(), m_s("") { }
        Foo(std::string const & s): Component(), m_s(s) { };

        std::string m_s;
    };

    int const INITIAL_X = 10;
    int const INITIAL_Y = 10;

    int const INITIAL_VX = 30;
    int const INITIAL_VY = 30;

    float const WORLD_DELTA = 30;
}


void SystemTests::run()
{
    int updates = 0;

    World world;

    world.setDelta(WORLD_DELTA);

    Begin("Register the MovementSystem (child of EntitySystem)");
        auto s = world.registerSystem<MovementSystem>();
        Assert(s != nullptr, "Failed to register MovementSystem");
        Assert(world.hasSystem<MovementSystem>(), "Failed to check is World has a MovementSystem.");
        Assert(!world.hasSystem<TestSystem>(), "World should not have a TestSystem yet.");
        Assert(s->m_registerComponentsCalled, "registerComponents has not been called");
        Assert(s->m_onRegisteredCalled, "onRegistered has not been called");
        Assert(s->componentBitMask().any(), "ComponentBitMask should not be 0.");
        Assert(s->componentBitMask().test(Position::BitId), "Position Bit should be set.");
        Assert(s->componentBitMask().test(Velocity::BitId), "Velocity Bit should be set.");
    End();


    Begin("Register the TestSystem (child of System)");
        auto s2 = world.registerSystem<TestSystem>();
        Assert(s2 != nullptr, "Failed to register TestSystem");
        Assert(world.hasSystem<TestSystem>(), "Failed to check is World has a TestSystem");
    End();

    Begin("Register the TestSystemWithParams (child of System)");
        auto swp2 = world.registerSystem<TestSystemWithParams>(42, 42.42);
        Assert(swp2 != nullptr, "Failed to register TestSystemWithParams");
        Assert(world.hasSystem<TestSystemWithParams>(), "Failed to check is World has a TestSystemWithParams");
    End();

    Begin("Retrieve the MovementSystem and the TestSystem");
        auto s3 = world.getSystem<MovementSystem>();
        auto s4 = world.getSystem<TestSystem>();
        Assert(s3 != nullptr, "Failed to retrieve MovementSystem");
        Assert(s4 != nullptr, "Failed to retrieve TestSystem");
    End();
 

    Begin("Create Entity and add required component for MovementSystem (before for onLoopStart())");
        auto e = world.newEntity();
        Assert(world.addComponent<Position>(e) != nullptr, "Failed to add Position to entity");
        Assert(world.addComponent<Velocity>(e) != nullptr, "Failed to add Velocity to entity");
        auto info = world.getEntityInfo(e);
        Assert(info.uniqueId() == e.uniqueId(), "Retrieved wrong EntityInfo");
        Assert(info.componentBitMask().any(), "ComponentBitMask should not be 0.");
        Assert(info.componentBitMask().test(Position::BitId), "Position bit should be set.");
        Assert(info.componentBitMask().test(Velocity::BitId), "Velocity bit should be set.");
        Assert(info.systemBitMask().none(), "EntityEntitySystemBitMask should be 0.");
    End();


    Begin("After onLoopStart() called (entity should be added to MovementSystem)");
        world.onLoopStart();
        info = world.getEntityInfo(e);
        Assert(info.componentBitMask().any(), "ComponentBitMask should not be 0.");
        Assert(info.componentBitMask().test(Position::BitId), "Position bit should be set.");
        Assert(info.componentBitMask().test(Velocity::BitId), "Velocity bit should be set.");
        Assert(info.systemBitMask().any(), "EntityEntitySystemBitMask should not be 0.");
        Assert(info.systemBitMask().test(MovementSystem::BitId), "Entity should be in the MovementSystem");
        Assert(s3->m_onEntityAddedCalled, "Callback onEntityAdded() not called");
        Assert(s3->m_lastAddedEntity == e.uniqueId(), "The wrong entity has been passed to the callback onEntityAdded()");
    End();


    Begin("Make a loop iteration (onLoopStart()) -> the entity must be moved by the MovementSystem");
       auto p = world.getComponent<Position>(e);
       auto v = world.getComponent<Velocity>(e);
       Assert(p != nullptr, "The entity does not have a Position component");
       Assert(v != nullptr, "The entity does not have a Velocity component");

       p->x = INITIAL_X;
       p->y = INITIAL_Y;
       v->x = INITIAL_VX;
       v->y = INITIAL_VY;

       Assert(!s3->m_onPreProcessingCalled, "Callback onPreProcessing() should not have been called yet.");
       Assert(!s3->m_onPostProcessingCalled, "Callback onPostProcessing() should not have been called yet.");
    
       world.onLoopStart();
       world.update();
       updates++;

       Assert(s3->m_onPreProcessingCalled, "Callback onPreProcessing() should have been called.");
       Assert(s3->m_onPostProcessingCalled, "Callback onPostProcessing() should have been called.");

       Assert(p->x == INITIAL_X + INITIAL_VX * WORLD_DELTA * updates, "Position.x has not been updated correctly");
       Assert(p->y == INITIAL_Y + INITIAL_VY * WORLD_DELTA * updates, "Position.y has not been updated correctly");

    End();


    Begin("Make another iteration");
        world.onLoopStart();
        s3->update();
        updates++;

        Assert(p->x == INITIAL_X + INITIAL_VX * WORLD_DELTA * updates, "Position.x has not been updated correctly");
        Assert(p->y == INITIAL_Y + INITIAL_VY * WORLD_DELTA * updates, "Position.y has not been updated correctly");
    End();


    Begin("Remove the Position -> removes the entity from MovementSystem");
        world.removeComponent<Position>(e);
        info = world.getEntityInfo(e);
        Assert(!info.componentBitMask().test(Position::BitId), "Position Bit should not be set.");
        Assert( info.componentBitMask().test(Velocity::BitId), "Velocity bit should be set.");
        Assert(!info.systemBitMask().test(MovementSystem::BitId), "Entity should not be in the MovementSystem");
        Assert(s3->m_onEntityRemovedCalled, "Callback onEntityRemoved() not called");
        Assert(s3->m_lastRemovedEntity == e.uniqueId(), "The wrong entity has been passed to the callback onRemovedAdded()");

        world.onLoopStart();
        s3->update();
        updates++;

       Assert(p->x == INITIAL_X + INITIAL_VX * WORLD_DELTA * (updates - 1), "Position.x should not have been updated.");
       Assert(p->y == INITIAL_Y + INITIAL_VY * WORLD_DELTA * (updates - 1), "Position.y should not have been updated.");

    End();


    Begin("Add the Position again -> add the entity to the MovementSystem");
        p = world.addComponent<Position>(e, INITIAL_X, INITIAL_Y);
        world.onLoopStart();
        info = world.getEntityInfo(e);
        Assert(p != nullptr, "Failed to add Position to entity");
        Assert(info.componentBitMask().any(), "ComponentBitMask should not be 0.");
        Assert(info.componentBitMask().test(Position::BitId), "Position bit should be set.");
        Assert(info.componentBitMask().test(Velocity::BitId), "Velocity bit should be set.");
        Assert(info.systemBitMask().any(), "EntityEntitySystemBitMask should not be 0.");
        Assert(info.systemBitMask().test(MovementSystem::BitId), "Entity should be in the MovementSystem");
    End();


    Begin("Update and queue for removal an entity between world.onLoopStart() and world.update()");
        updates = 0;
        world.onLoopStart();
        world.addComponent<Foo>(e, "Foo");
        world.remove(e);
        s3->update();
        updates++;

        Assert(p->x == INITIAL_X + INITIAL_VX * WORLD_DELTA * updates, "Position.x should have been updated.");
        Assert(p->y == INITIAL_Y + INITIAL_VY * WORLD_DELTA * updates, "Position.y should have been updated.");

        world.onLoopStart(); // Now the removed entity is actually removed
        world.update();
        updates++;

        Assert(p->x == INITIAL_X + INITIAL_VX * WORLD_DELTA * (updates - 1), "Position.x should not have been updated.");
        Assert(p->y == INITIAL_Y + INITIAL_VY * WORLD_DELTA * (updates - 1), "Position.y should not have been updated.");
    End();

    Begin("Recreate the entity -> add the entity to the MovementSystem");
        e = world.newEntity();
        p = world.addComponent<Position>(e, INITIAL_X, INITIAL_Y);
        v = world.addComponent<Velocity>(e, INITIAL_VX, INITIAL_VY);
        world.onLoopStart();
        info = world.getEntityInfo(e);
        Assert(p != nullptr, "Failed to add Position to entity");
        Assert(v != nullptr, "Failed to add Velocity to entity");
        Assert(info.componentBitMask().any(), "ComponentBitMask should not be 0.");
        Assert(info.componentBitMask().test(Position::BitId), "Position bit should be set.");
        Assert(info.componentBitMask().test(Velocity::BitId), "Velocity bit should be set.");
        Assert(info.systemBitMask().any(), "EntityEntitySystemBitMask should not be 0.");
        Assert(info.systemBitMask().test(MovementSystem::BitId), "Entity should be in the MovementSystem");
    End();

    Begin("Update and remove an entity between world.onLoopStart() and world.update()");
        updates = 0;
        world.onLoopStart();
        world.addComponent<Foo>(e, "Foo");
        world.remove(e, true);
        s3->update();
        updates++;

        Assert(p->x == INITIAL_X + INITIAL_VX * WORLD_DELTA * (updates - 1), "Position.x should not have been updated.");
        Assert(p->y == INITIAL_Y + INITIAL_VY * WORLD_DELTA * (updates - 1), "Position.y should not have been updated.");
    End();

    Begin("Remove the MovementSystem");
        // Recreate an entity
        e = world.newEntity();
        p = world.addComponent<Position>(e, INITIAL_X, INITIAL_Y);
        v = world.addComponent<Velocity>(e, INITIAL_VX, INITIAL_VY);
        world.onLoopStart();

        info = world.getEntityInfo(e);

        Assert(p != nullptr, "Failed to add Position to entity");
        Assert(v != nullptr, "Failed to add Velocity to entity");
        Assert(info.componentBitMask().any(), "ComponentBitMask should not be 0.");
        Assert(info.componentBitMask().test(Position::BitId), "Position bit should be set.");
        Assert(info.componentBitMask().test(Velocity::BitId), "Velocity bit should be set.");
        Assert(info.systemBitMask().any(), "EntityEntitySystemBitMask should not be 0.");
        Assert(info.systemBitMask().test(MovementSystem::BitId), "Entity should be in the MovementSystem");

        updates = 0;
        world.onLoopStart();
        world.update();
        updates++;

        Assert(p->x == INITIAL_X + INITIAL_VX * WORLD_DELTA * updates, "Position.x should have been updated.");
        Assert(p->y == INITIAL_Y + INITIAL_VY * WORLD_DELTA * updates, "Position.y should have been updated.");

        world.removeSystem<MovementSystem>();
        world.onLoopStart();
        world.update();
        updates++;

        info = world.getEntityInfo(e);

        Assert(world.getSystem<MovementSystem>() == nullptr, "MovementSystem has not been removed");
        Assert(p->x == INITIAL_X + INITIAL_VX * WORLD_DELTA * (updates - 1), "Position.x should not have been updated.");
        Assert(p->y == INITIAL_Y + INITIAL_VY * WORLD_DELTA * (updates - 1), "Position.y should not have been updated.");
        Assert(info.systemBitMask().none(), "EntityEntitySystemBitMask should be 0.");
        Assert(!info.systemBitMask().test(MovementSystem::BitId), "Entity should not be in the MovementSystem");
    End();

    Begin("Add an EntitySystem with entities already added in the world");
        world.registerSystem<EntitySystemWithParams>(42, 42.42);
        info = world.getEntityInfo(e);
        Assert(info.systemBitMask().any(), "EntitySystemBitMask should not be 0.");
        Assert(info.systemBitMask().test(EntitySystemWithParams::BitId), "Entity should be in the EntitySystemWithParams");
    End();
}
