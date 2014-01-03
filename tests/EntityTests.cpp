#include "EntityTests.hpp"

#include <HybridECS.hpp>

using namespace Hybrid::ECS;

EntityTests::EntityTests(): Test("Entity tests")
{

}

void EntityTests::run()
{
    World world;

    Begin("Creating an entity");
        auto const & e = world.newEntity();
        Assert(world.isAlive(e), "e should be alive");
        Assert(world.count() == 1, "The world should contain 1 entity");
        Assert(world.totalCreated() == 1, "1 entity should have been created in total");
        Assert(world.totalRemoved() == 0, "0 entity should have been removed in total");
    End();


    Begin("Retrieve EntityInfo");
        auto info = world.getEntityInfo(e);
        Assert(info.id() == e.id(), "Retrieved wrong EntityInfo : ids are different");
        Assert(info.uniqueId() == e.uniqueId(), "Retrieved wrong EntityInfo");
        Assert(info.alive(), "Entity should be alive");
        Assert(info.componentBitMask().none(), "ComponentBitMask should be 0.");
        Assert(info.systemBitMask().none(), "EntityEntitySystemBitMask should be 0.");
    End();

    
    Begin("Creating a second entity");
        auto const & f = world.newEntity();
        Assert(world.isAlive(f), "f should be alive");
        Assert(e.id() != f.id(), "The entities should have different ids");
        Assert(e.uniqueId() != f.uniqueId(), "The entities should have different unique ids");
        Assert(world.count() == 2, "The world should contain 2 entities");
        Assert(world.totalCreated() == 2, "2 entities should have been created in total");
        Assert(world.totalRemoved() == 0, "0 entity should have been removed in total");
    End();


    auto id = e.id();
    auto uniqueId = e.uniqueId();

    Begin("Remove the entity w/o updating the World");
        world.remove(e);
        Assert(world.isAlive(e), "e should still be alive");
        Assert(world.count() == 2, "The world should contain 2 entities");
        Assert(world.totalCreated() == 2, "2 entities should have been created in total");
        Assert(world.totalRemoved() == 0, "0 entity should have been removed in total");
    End();


    Begin("Remove the entity and update the World");
        world.remove(e);
        world.onLoopStart();
        Assert(!world.isAlive(e), "e should be dead");
        Assert(world.count() == 1, "The world should contain 1 entity");
        Assert(world.totalCreated() == 2, "2 entities should have been created in total");
        Assert(world.totalRemoved() == 1, "1 entity should have been removed in total");
    End();


    Begin("Creating a third entity");
        auto const & g = world.newEntity();
        Assert(world.isAlive(g), "g should be alive");
        Assert(id == g.id(), "The new entity should have the same id as the last removed entity");
        Assert(uniqueId != g.uniqueId(), "The entities should all have different unique ids");
        Assert(world.count() == 2, "The world should contain 2 entities");
        Assert(world.totalCreated() == 3, "3 entities should have been created in total");
    End();
}
