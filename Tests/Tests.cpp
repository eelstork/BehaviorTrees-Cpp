#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp> //single-header
#include "status.h" // project being tested
#include <string>

BOOST_AUTO_TEST_CASE(status_constructor_no_arg) { 
	BOOST_CHECK( status().running() ); 
}

BOOST_AUTO_TEST_CASE(status_complete) {
	BOOST_CHECK(!status(-1).complete());
	BOOST_CHECK(!status( 0).complete());
	BOOST_CHECK( status( 1).complete());
}

BOOST_AUTO_TEST_CASE(status_failing) {
	BOOST_CHECK( status(-1).failing());
	BOOST_CHECK(!status( 0).failing());
	BOOST_CHECK(!status( 1).failing());
}

BOOST_AUTO_TEST_CASE(status_impending) {
	BOOST_CHECK( status(-1).impending());
	BOOST_CHECK( status( 0).impending());
	BOOST_CHECK(!status( 1).impending());
}

BOOST_AUTO_TEST_CASE(status_pending) {
	BOOST_CHECK(!status(-1).pending());
	BOOST_CHECK( status( 0).pending());
	BOOST_CHECK( status( 1).pending());
}

BOOST_AUTO_TEST_CASE(status_running) {
	BOOST_CHECK(!status(-1).running());
	BOOST_CHECK( status( 0).running());
	BOOST_CHECK(!status( 1).running());
}

BOOST_AUTO_TEST_CASE(status_const_done) { BOOST_CHECK( done.complete() ); }
BOOST_AUTO_TEST_CASE(status_const_fail) { BOOST_CHECK( fail.failing()  ); }
BOOST_AUTO_TEST_CASE(status_const_cont) { BOOST_CHECK( cont.running()  ); }