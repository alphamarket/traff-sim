#ifndef MANIFEST_HPP
#define	MANIFEST_HPP

#include "lizard/lizard.hpp"

// include test case files
#include "tests/basic/car_tester.hpp"
#include "tests/basic/joint_tester.hpp"
#include "tests/basic/street_tester.hpp"

void lizard_manifest() {
    /*
     * use following macros to register test cases
     *      lizard_register(<test class name>)
     *      lizard_register_custom(<test label>, <test class name>)
     * example:
     *      lizard_register(test::template_tester);
     */
     lizard_register(test::car_basic_tester);
     lizard_register(test::street_basic_tester);
     lizard_register(test::joint_basic_tester);
}

#endif	/* MANIFEST_HPP */
