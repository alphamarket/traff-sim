#ifndef MANIFEST_HPP
#define	MANIFEST_HPP
#include "hpp/teststrap.hpp"
#include "hpp/registery.hpp"
/*
 * Include test case files
 */
#include "TestCases/basic/carTestCase.hpp"
#include "TestCases/basic/jointTestCase.hpp"
#include "TestCases/basic/streetTestCase.hpp"
namespace CPP_TESTER {
    /**
     * bootstrap the test suite for testing
     */
    void __bootstrap() {
        /* example */
        registery::__register("[BASIC] Car Test",   new TESTS::BASIC::carTestCase());
        registery::__register("[BASIC] Street Test",new TESTS::BASIC::streetTestCase());
        registery::__register("[BASIC] Joint Test",new TESTS::BASIC::jointTestCase());
    }
}
#endif	/* MANIFEST_HPP */
