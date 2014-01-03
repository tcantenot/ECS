#include "GroupTests.hpp"

#include <HybridECS.hpp>

#include <sstream>

using namespace Hybrid::ECS;

GroupTests::GroupTests(): Test("Group tests")
{

}

void GroupTests::run()
{
    World world;

    Begin("Creating an entity");
        auto const & e = world.newEntity();
        Assert(world.isAlive(e), "e should be alive");
        Assert(world.count() == 1, "The world should contain 1 entity");
        Assert(world.totalCreated() == 1, "1 entity should have been created in total");
        Assert(world.totalRemoved() == 0, "0 entity should have been removed in total");
    End();

    Begin("Add entity to one group");
        world.addToGroup(e, "grp1");
        Assert(world.isInGroup(e, "grp1"), "Entity should be in group grp1");
        auto const & entities = world.getEntitiesByGroup("grp1");
        Assert(entities.size() == 1, "Should retrieve only one entity");
        Assert(entities[0] == e, "Failed to retrieve entity");
        auto const & groups = world.getGroupsByEntity(e);
        Assert(groups.size() == 1, "Group size should be 1");
        Assert(groups[0] == "grp1", "Failed to retrieve group");
    End();

    Begin("Add entity to other groups");
        world.addToGroup(e, "grp2");
        world.addToGroup(e, "grp3");
        Assert(world.isInGroup(e, "grp2"), "Entity should be in group grp2");
        Assert(world.isInGroup(e, "grp3"), "Entity should be in group grp3");
        auto const & v1 = world.getEntitiesByGroup("grp2");
        Assert(v1.size() == 1, "Should retrieve only one entity");
        Assert(v1[0] == e, "Failed to retrieve entity");
        auto const & v2 = world.getEntitiesByGroup("grp3");
        Assert(v2.size() == 1, "Should retrieve only one entity");
        Assert(v2[0] == e, "Failed to retrieve entity");
        auto const & grp = world.getGroupsByEntity(e);
        Assert(grp.size() == 3, "Group size should be 3");
        for(std::size_t i = 0; i < grp.size(); ++i)
        {
            std::ostringstream oss;
            oss << "grp" << (i + 1);
            Assert(grp[i] == oss.str(), "Failed to retrieve group" + oss.str());
        }
    End();
    
    Begin("Creating a second entity");
        auto const & f = world.newEntity();
        Assert(world.isAlive(f), "f should be alive");
        Assert(world.count() == 2, "The world should contain 2 entities");
    End();

    Begin("Creating a third entity");
        auto const & g = world.newEntity();
        Assert(world.isAlive(g), "g should be alive");
        Assert(world.count() == 3, "The world should contain 3 entities");
    End();

    Begin("Add entities to the grp1 and retrieve them");
        world.addToGroup(f, "grp1");
        world.addToGroup(g, "grp1");
        Assert(world.isInGroup(f, "grp1"), "f should be in grp 1");
        Assert(world.isInGroup(g, "grp1"), "g should be in grp 1");
        auto const & v3 = world.getEntitiesByGroup("grp1");
        Assert(v3.size() == 3, "Should retrieve 3 entities");
        Assert(v3[0] == e, "Failed to retrieve entity 1");
        Assert(v3[1] == f, "Failed to retrieve entity 2");
        Assert(v3[2] == g, "Failed to retrieve entity 3");
    End();

    Begin("Remove from group");
        world.removeFromGroup(e, "grp2");
        Assert(world.isInGroup(e, "grp2") == false, "e should not be in grp2");
        auto const & v4 = world.getEntitiesByGroup("grp2");
        Assert(v4.empty(), "There should be no entity in grp2");
        Assert(world.isInGroup(e, "grp1"), "e should still be in grp1");
        Assert(world.isInGroup(e, "grp3"), "e should still be in grp3");
    End();

    Begin("Remove from all groups");
        world.removeFromAllGroups(e);
        Assert(world.isInGroup(e, "grp1") == false, "e should not be in grp1");
        Assert(world.isInGroup(e, "grp3") == false, "e should not be in grp3");
        auto const & v5 = world.getEntitiesByGroup("grp1");
        auto const & v6 = world.getEntitiesByGroup("grp3");
        Assert(v5.size() == 2, "grp1 should contain 2 entities");
        Assert(v6.empty(), "grp3 should be empty");
    End();
}

