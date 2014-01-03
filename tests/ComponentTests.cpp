#include "ComponentTests.hpp"

#include <HybridECS.hpp>

using namespace Hybrid::ECS;

ComponentTests::ComponentTests(): Test("Component tests")
{

}

void ComponentTests::run()
{
    World world;

    struct Position : public Component<Position>
    {
        Position(int x = 0, int y = 0): x(x), y(y) { }
        int x, y;
    };

    struct Foo : public Component<Foo>
    {
        Foo(int i = 0): m_i(i) { }

        int m_i;
    };

    struct Useless : public Component<Useless>
    {

    };

    Begin("Creating an entity");
        auto const & e = world.newEntity();
        Assert(world.isAlive(e), "e should be alive");
    End();

    Begin("Creating another entity");
        auto const & f = world.newEntity();
        Assert(world.isAlive(f), "f should be alive");
    End();

    Begin("Adding a DefaultConstructible component to the entity");
        auto pos = world.addComponent<Position>(e);
        auto posf = world.addComponent<Position>(f);
        Assert(pos != nullptr, "pos is nullptr");
        Assert(posf != nullptr, "posf is nullptr");
    End();

    pos = world.getComponent<Position>(e);
    int x(6), y(7);
    pos->x = x;
    pos->y = y;

    posf = world.getComponent<Position>(f);
    posf->x = x + 1;
    posf->y = y + 1;

    Begin("Retrieving a component container (Position container)");
        auto vec = world.getAllComponents<Position>();
        Assert(vec.size() == 2, "The Position container should contain 2 items");
        Assert(vec[0].x == x && vec[0].y == y, "Wrong Position in vec[0]");
        Assert(vec[1].x == (x + 1) && vec[1].y == (y + 1), "Wrong Position in vec[1]");
    End();

    Begin("Retrieving an inexistant component container");
        auto ivec = world.getAllComponents<Useless>();
        Assert(ivec.size() == 0, "The Useless container should contain 0 item.");
    End();

    Begin("Adding a Non-DefaultConstructible component to the entity");
        auto foo = world.addComponent<Foo>(e, 42);
        Assert(foo != nullptr, "foo is nullptr");
    End();

    Begin("Getting an existing component");
        auto pos2 = world.getComponent<Position>(e);
        Assert(pos2 != nullptr, "Failed to get Position");
        Assert(pos2->x == pos->x && pos2->y == pos->y && pos2->x == x && pos2->y == y, "Got the wrong Position component");
        pos2->x = 166;
        Assert(pos2->x == pos->x, "The pointers point to different Position components");

        auto foo2 = world.getComponent<Foo>(e);
        Assert(foo2 != nullptr, "Failed to get Foo component");
        Assert(foo2->m_i == foo->m_i, "Got the wrong Foo Component");
        foo2->m_i = 42;
        Assert(foo2->m_i == foo->m_i, "The pointers point to different Foo components");
    End();

    Begin("Check the presence of component");
        Assert(world.hasComponent<Position>(e), "e should have a Position component");
        Assert(world.hasComponent<Foo>(e), "e should have a Foo component");
        Assert(!world.hasComponent<Useless>(e), "e should not have a Useless component");
    End();

    Begin("Getting an non-existing component");
        auto u = world.getComponent<Useless>(e);
        Assert(u == nullptr, "Found an inexistant component..");
    End();

    Begin("Remove an existing component");
        world.removeComponent<Foo>(e);        
        Assert(world.getComponent<Foo>(e) == nullptr, "The Foo component has not be removed");
        Assert(world.getComponent<Position>(e) != nullptr, "The Position has been removed with Foo..");
        auto info = world.getEntityInfo(e);
        Assert(info.componentBitMask().test(Foo::BitId) == false, "The Foo component has not been removed from componentMask");
    End();

    Begin("Remove an non-existing component");
        world.removeComponent<Useless>(e);
        Assert(world.getComponent<Position>(e) != nullptr, "The Position has been removed with Useless..");
        world.removeComponent<Foo>(e);
        Assert(world.getComponent<Position>(e) != nullptr, "The Position has been removed with Foo..");
    End();


    Begin("Replace an existing component (a component is unique)");
        auto pos3 = world.addComponent<Position>(e);
        Assert(pos3 != nullptr, "pos3 is nullptr");
        int newX(10), newY(11);
        pos3->x = newX;
        pos3->y = newY;
        auto pp = world.getComponent<Position>(e);
        Assert(pp->x == newX && pp->y == newY, "The Position has not been replaced by the new one");
    End();

    Begin("Remove all components from entity");
        world.removeAllComponents(e);
        Assert(world.getComponent<Position>(e) == nullptr, "The Position component has not been removed");
        Assert(world.getComponent<Foo>(e) == nullptr, "The Foo component has not been removed");
    End();

    Begin("Creating a third entity");
        auto const & g = world.newEntity();
        Assert(world.isAlive(g), "g should be alive");
        Assert(e.id() != g.id(), "The entities should have different ids");
        Assert(e.uniqueId() != g.uniqueId(), "The entities should have different unique ids");
    End();

    Begin("Adding a Position component to the second entity");
        auto pos4 = world.addComponent<Position>(g);
        Assert(pos4 != nullptr, "pos4 is nullptr");
        pos4->x = 2;
        pos4->y = 3;
        auto pos5 = world.getComponent<Position>(g);
        Assert(pos5->x == pos4->x && pos5->y == pos4->y, "Got the wrong Position component");
        world.removeAllComponents(e);
        Assert(world.getComponent<Position>(e) == nullptr, "e should not have Positon component");
    End();
}
