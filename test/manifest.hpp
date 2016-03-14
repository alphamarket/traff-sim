#ifndef MANIFEST_HPP
#define	MANIFEST_HPP
#include "hpp/teststrap.hpp"
#include "hpp/registery.hpp"
/*
 * Include test case files
 */
// #include "TestCases/example/fooTestCase.hpp"
namespace CPP_TESTER {
    /**
     * bootstrap the test suite for testing
     */
    void __bootstrap() {
        /* example */
        // registery::__register("Example Tester", new fooTestCase());
    }
}
#endif	/* MANIFEST_HPP */
