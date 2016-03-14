#ifndef REGISTERY_HPP
#define	REGISTERY_HPP
#include "testCase.hpp"
#include <vector>
#include <stdexcept>
namespace CPP_TESTER {
    typedef std::string label;
    typedef vector< void* > test_args;
    typedef pair< testCase*,  test_args > test_pack;
    typedef pair< label, test_pack > test_case;
    typedef vector<test_case> test_suite;
    class registery {
    protected:
            static test_suite __suite;
    public:
        static inline void __register(label __label, testCase* __testCase) { registery::__register(__label, __testCase, test_args()); }
        static void __register(label __label, testCase* __testCase, test_args __args) {
            if(__testCase == NULL)
                throw std::invalid_argument("The test case instance cannot be null...");
            __suite.push_back(test_case(__label, test_pack (__testCase, __args )));
        }
        static inline test_suite getTestSuites() { return __suite; }
        static inline void clear() { __suite.clear(); }
    };
    test_suite registery::__suite;
}
#endif	/* REGISTERY_HPP */