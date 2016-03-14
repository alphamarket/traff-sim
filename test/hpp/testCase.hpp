#ifndef TESTER_HPP
#define	TESTER_HPP
#include "teststrap.hpp"
namespace CPP_TESTER {
    class testCase {
    public:
        /**
         * @brief The virtual destructor
         */
        virtual ~testCase()     { }
        /**
         * @brief Runs before the run()
         */
        virtual void pre_run()  { }
        /**
         * @brief Runs after the run()
         */
        virtual void post_run() { }
        /**
         * @brief Runs the testcase
         */
        virtual void run(size_t argc = 0, void** argv = NULL) = 0;
    };
}
#endif	/* TESTER_HPP */
