#include <vector>
#include <iostream>
#include <cassert>

#include <HybridECS.hpp>

#include "../tests/Test.hpp"
#include "../tests/EntityTests.hpp"
#include "../tests/ComponentTests.hpp"
#include "../tests/SystemTests.hpp"
#include "../tests/GroupTests.hpp"
#include "../tests/VariableTests.hpp"

using namespace Hybrid::ECS;

int main(void)
{
    std::vector<Test *> tests;

    tests.push_back(new EntityTests());
    tests.push_back(new ComponentTests());
    tests.push_back(new SystemTests());
    tests.push_back(new GroupTests());
    tests.push_back(new VariableTests());

    unsigned int nbSuccess = 0;

    for(auto t : tests)
    {
        std::cout << "\n>>>>> <" << t->name() << "> <<<<<" << std::endl << std::endl << std::endl;

        t->run();

        std::cout << "\nResults : " << t->nbTestsSucceeded() << " / " << t->nbTestsRun() << " tests PASSED ";

        auto nbFailures = t->nbTestsRun() - t->nbTestsSucceeded();

        std::cout << " => ";
        
        if(nbFailures == 0)
        {
            std::cout << "CONGRATULATIONS !" << std::endl;
            ++nbSuccess;
        }
        else
        {
            std::cout << nbFailures << " test" << (nbFailures > 1 ? "s" : "") << " FAILED" << std::endl;
        }

        std::cout << std::endl << "----------------------------------------------------------------" << std::endl;
    }

    std::cout << "##### SUMMARY #####" << std::endl << std::endl;

    if(nbSuccess == tests.size())
    {
        std::cout << "CONGRATULATIONS : ALL TESTS PASSED !" << std::endl;
    }
    else
    {
        std::cout << "Results : " << nbSuccess << " / " << tests.size() << " tests PASSED";
        auto nbFails = tests.size() - nbSuccess;
        std::cout << " => " << nbFails << " test" << (nbFails > 1 ? "s" : "") << " FAILED" << std::endl;
    }

    std::cout << std::endl;

    for(auto t : tests)
    {
        delete t;
    }

    return 0;
}
