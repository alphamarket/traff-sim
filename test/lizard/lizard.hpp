#ifndef LIZARD_HPP
#define	LIZARD_HPP

#include <exception>
#include <assert.h>
#include <typeinfo>
#if __cplusplus >= 201103L
#   include <type_traits>
#endif
#include "stdafx.hpp"
/**
 * Makes sure the input is true
 */
#define BESURE(o) assert((o))
/**
 * Failure assertion with message
 */
#define FAIL(o) __assert_fail (__STRING(o), __FILE__, __LINE__, __ASSERT_FUNCTION)
/**
 * Makes sure the arguments are eqaul
 */
#define IS_EQUAL(o, z) BESURE((o) == (z))
/**
 * Makes sure the arguments are NOT eqaul
 */
#define NOT_EQUAL(o, z) BESURE((o) != (z))
/**
 * Makes sure the arguments are eqaul
 */
#define SHOULD_BE(o, z) IS_EQUAL(o, z)
/**
 * Makes sure the arguments are NOT eqaul
 */
#define SHOULD_NOT_BE(o, z) NOT_EQUAL(o, z)
/**
 * Makes sure the arguments are eqaul
 */
#define IS_SAME(o, z) IS_EQUAL(o, z)
/**
 * Makes sure the arguments are NOT eqaul
 */
#define NOT_SAME(o, z) NOT_EQUAL(o, z)
/**
 * Makes sure the argument is NOT null
 */
#define NOT_NULL(o) NOT_EQUAL(o,  NULL)
/**
 * Makes sure the argument is NULL
 */
#define IS_NULL(o) IS_EQUAL(o,  NULL)
/**
 * Makes sure the argument is IS_EQUAL to zero
 */
#define IS_ZERO(o) IS_EQUAL(o, 0)
/**
 * Makes sure the argument is NOT IS_EQUAL to zero
 */
#define NOT_ZERO(o) NOT_EQUAL(o, 0)
/**
 * Makes sure the argument is true
 */
#define IS_TRUE(o) IS_EQUAL(o, true)
/**
 * Makes sure the argument is NOT true
 */
#define NOT_TRUE(o) IS_EQUAL(o, false)
/**
 * Makes sure the argument is false
 */
#define IS_FALSE(o) IS_EQUAL(o, false)
/**
 * Makes sure the argument is NOT false
 */
#define NOT_FALSE(o) IS_EQUAL(o, true)
/**
 * Makes sure the arguments are same type
 */
#define IS(o, z) IS_EQUAL(typeid(o).name(), typeid(z).name())
/**
 * Makes sure the arguments are NOT same type
 */
#define IS_NOT(o, z) NOT_EQUAL(typeid(o).name(), typeid(z).name())
/**
 * Makes sure the argument is a pointer
 */
#define IS_POINTER(o) IS_EQUAL(typeid(o).name()[0], 'P')
/**
 * Makes sure the argument is NOT a pointer
 */
#define NOT_POINTER(o) NOT_EQUAL(typeid(o).name()[0], 'P')
/**
 * Makes sure the arguments WHICH ARE POINTERS are pointing same location
 */
#define PSAME_POINT(o, z) IS_SAME(o, z)
/**
 * Makes sure the arguments WHICH ARE {NOT} POINTERS are pointing same location
 */
#define NSAME_POINT(o, z) IS_SAME(&o, &z)
/**
 * Makes sure the arguments WHICH ARE POINTERS are NOT pointing same location
 */
#define PNOT_SAME_POINT(o, z) NOT_SAME(o, z)
/**
 * Makes sure the arguments WHICH ARE {NOT} POINTERS are NOT pointing same location
 */
#define NNOT_SAME_POINT(o, z) NOT_SAME(&o, &z);
/**
 * Makes sure if after executing the passed argument an exception will be thrown
 */
#define SHOULD_THROW(o) \
try { \
    o; \
    FAIL("Expecting to catch something, but didn't!"); \
} catch(...) { }
/**
 * Makes sure if after executing the passed argument an exception does NOT get thrown
 */
#define SHOULD_NOT_THROW(o) \
try { \
    o; \
} catch(...) { FAIL("Didn't expect to catch exception, but did!"); }
/**
 * Makes sure the arguments are eqaul
 */
#define ASSERT_IS_EQUAL(o, z) IS_EQUAL(o, z)
/**
 * Makes sure the arguments are NOT eqaul
 */
#define ASSERT_NOT_EQUAL(o, z) NOT_EQUAL(o, z)
/**
 * Makes sure the arguments are eqaul
 */
#define ASSERT_SAME(o, z) IS_SAME(o, z)
/**
 * Makes sure the arguments are NOT eqaul
 */
#define ASSERT_NOT_SAME(o, z) NOT_SAME(o, z)
/**
 * Makes sure the argument is NOT null
 */
#define ASSERT_NOT_NULL(o) NOT_NULL(o)
/**
 * Makes sure the argument is NULL
 */
#define ASSERT_NULL(o) IS_NULL(o)
/**
 * Makes sure the argument is IS_EQUAL to zero
 */
#define ASSERT_ZERO(o) IS_ZERO(o)
/**
 * Makes sure the argument is NOT IS_EQUAL to zero
 */
#define ASSERT_NOT_ZERO(o) NOT_ZERO(o)
/**
 * Makes sure the argument is true
 */
#define ASSERT_TRUE(o) IS_TRUE(o)
/**
 * Makes sure the argument is NOT true
 */
#define ASSERT_NOT_TRUE(o) NOT_TRUE(o)
/**
 * Makes sure the argument is false
 */
#define ASSERT_FALSE(o) IS_FALSE(o)
/**
 * Makes sure the argument is NOT false
 */
#define ASSERT_NOT_FALSE(o) NOT_FALSE(o)
/**
 * Makes sure the arguments are same type
 */
#define ASSERT_IS(o, z) IS(o, z)
/**
 * Makes sure the arguments are NOT same type
 */
#define ASSERT_IS_NOT(o, z) IS_NOT(o, z)
/**
 * Makes sure the argument is a pointer
 */
#define ASSERT_POINTER(o) IS_POINTER(o)
/**
 * Makes sure the argument is NOT a pointer
 */
#define ASSERT_NOT_POINTER(o) NOT_POINTER(o)
/**
 * Makes sure the arguments WHICH ARE POINTERS are pointing same location
 */
#define ASSERT_PSAME_POINT(o, z) PSAME_POINT(o, z)
/**
 * Makes sure the arguments WHICH ARE {NOT} POINTERS are pointing same location
 */
#define ASSERT_NSAME_POINT(o, z) NSAME_POINT(o, z)
/**
 * Makes sure the arguments WHICH ARE POINTERS are pointing same location
 */
#define ASSERT_PNOT_SAME_POINT(o, z) PNOT_SAME_POINT(o, z)
/**
 * Makes sure the arguments WHICH ARE {NOT} POINTERS are pointing same location
 */
#define ASSERT_NNOT_SAME_POINT(o, z) NNOT_SAME_POINT(o, z)
/**
 * Failure assertion
 */
#define ASSERT_FAILURE FAIL

#if __cplusplus >= 201103L
/**
 * Makes sure the "o" is inherited from "b"
 */
#define IS_BASE_OF(b, o) 							\
  ((std::is_base_of<b, o>::value)								\
   ? __ASSERT_VOID_CAST (0)						\
   : FAIL("type inheritance assertion failure!"))
#endif

#include "utilities.hpp"

#endif	/* LIZARD_HPP */
