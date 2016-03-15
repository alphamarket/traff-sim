#ifndef MANIFEST_HPP
#define	MANIFEST_HPP

#include "lizard/lizard.hpp"

// include test case files
#include "tests/basic/car_tester.hpp"
#include "tests/basic/joint_tester.hpp"
#include "tests/basic/street_tester.hpp"
#include "tests/basic/joint_kill_tester.hpp"
#include "tests/basic/joint_hold_tester.hpp"

void lizard_manifest() {
     lizard_register(test::car_basic_tester);
     lizard_register(test::street_basic_tester);
     lizard_register(test::joint_basic_tester);
     lizard_register(test::joint_kill_basic_tester);
     lizard_register(test::joint_hold_basic_tester);
}

#endif	/* MANIFEST_HPP */
