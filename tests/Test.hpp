#ifndef TEST_HPP
#define TEST_HPP

#include <string>
#include <iostream>

class Test
{
    public:
        Test(std::string const & name);
        virtual ~Test();

        virtual void run() = 0;

        virtual std::string const & name() const;

        std::size_t nbTestsRun() const;
        std::size_t nbTestsSucceeded() const;

    protected:
        void Begin(std::string const & info);

        void Assert(bool predicat, std::string const & msg);

        void End();

        std::string m_name;
        std::size_t m_nbTestsRun;
        std::size_t m_nbTestsSucceeded;

    private:
        bool m_testSucceeded;
};

#endif //TEST_HPP
