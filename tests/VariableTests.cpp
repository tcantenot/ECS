#include "VariableTests.hpp"

#include <HybridECS.hpp>

#include <limits>
#include <cmath>

using namespace Hybrid::ECS;

template <typename T>
static bool equal(T const & lhs, T const & rhs)
{
    return std::abs(lhs - rhs) <= std::numeric_limits<T>::epsilon();
}

VariableTests::VariableTests(): Test("Variable tests")
{

}

void VariableTests::run()
{
    World world;

    Begin("Add & retrieve variable");
        world.setValue<int>("int", 42);
        Assert(equal(world.getValue<int>("int"), 42), "Failed to retrieve int variable");

        world.setValue<long>("long", 4200);
        Assert(equal<long>(world.getValue<long>("long"), 4200), "Failed to retrieve long variable");

        //world.setValue("string", "this is a string");
        //Assert(world.getValue<std::string>("string") == "this is a string", "Failed to retrieve std::string variable");
        
        world.setValue<float>("float", 42.424242);
        Assert(equal<float>(world.getValue<float>("float"), 42.424242), "Failed to retieve float variable");

        world.setValue<bool>("bool_true", true);
        Assert(world.getValue<bool>("bool_true") == true, "Failed to retrieve bool true variable");

        world.setValue<bool>("bool_false", false);
        Assert(world.getValue<bool>("bool_false") == false, "Failed to retrieve bool false variable");
    End();

    Begin("Reset an existing variable");
        world.setValue<bool>("running", true);
        Assert(world.getValue<bool>("running") == true, "Failed to retrieve variable");

        world.setValue<bool>("running", false);
        Assert(world.getValue<bool>("running") == false, "Failed to reset variable");
    End();

}
