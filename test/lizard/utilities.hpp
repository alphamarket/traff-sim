#ifndef LIZARD_UTILITIES_HPP
#define	LIZARD_UTILITIES_HPP

#include "stdafx.hpp"

class lizard_base_tester {
public:
    /**
     * @brief Runs the testcase
     */
    virtual void run() = 0;
    /**
     * @brief Runs before the run()
     */
    virtual void pre_run()  { }
    /**
     * @brief Runs after the run()
     */
    virtual void post_run() { }
    /**
     * @brief The virtual destructor
     */
    virtual ~lizard_base_tester()     { }
};

typedef pair< string, lizard_base_tester*> test_case;

vector<test_case> lizard_registery;

inline void add2registery(string label, lizard_base_tester* tester)
{ extern vector<test_case> lizard_registery; lizard_registery.push_back(test_case(label, tester)); }

#define lizard_register_custom(label, name) add2registery(label, new name)
#define lizard_register(name) lizard_register_custom(string(#name), name)

#endif	/* LIZARD_UTILITIES_HPP */
