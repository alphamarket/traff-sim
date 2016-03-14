#ifndef MANIFEST_HPP
#define	MANIFEST_HPP
#include "hpp/teststrap.hpp"
#include "hpp/registery.hpp"
/*
 * Include test case files
 */
 #include "TestCases/basic/carTestCase.hpp"
namespace CPP_TESTER {
    /**
     * bootstrap the test suite for testing
     */
    void __bootstrap() {
        /* example */
        registery::__register("[BASIC] Car Test", new TESTS::BASIC::carTestCase());
    }
}
#endif	/* MANIFEST_HPP */
