#include "Test.hpp"


Test::Test(std::string const & name):
    m_name(name), m_nbTestsRun(0), m_nbTestsSucceeded(0),
    m_testSucceeded(true)
{

}

Test::~Test()
{

}

std::string const & Test::name() const
{
    return m_name;
}

std::size_t Test::nbTestsRun() const
{
    return m_nbTestsRun;
}

std::size_t Test::nbTestsSucceeded() const
{
    return m_nbTestsSucceeded;
}

void Test::Begin(std::string const & info)
{
    std::cout << "### " << info << " ###" << std::endl;
}

void Test::Assert(bool predicat, std::string const & msg)
{
    m_testSucceeded &= predicat;

    if(!predicat)
    {
        std::cout << " ~> Assertion failed : " << msg << std::endl;
    }
}


void Test::End()
{
    if(m_testSucceeded)
    {
        std::cout << " => SUCCESS" << std::endl << std::endl;
        ++m_nbTestsSucceeded;
    }
    else
    {
        std::cout << " => FAILURE" << std::endl << std::endl;
    }

    ++m_nbTestsRun;
    m_testSucceeded = true;
}


