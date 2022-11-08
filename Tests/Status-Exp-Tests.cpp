#define BOOST_TEST_MODULE BehaviorTreeTests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
//
#include "status.h"
#include "status-exp.h"
#include "status-pexp.h"
#include <string>
#include <iostream>
using namespace std;

namespace bdata = boost::unit_test::data;

//
// Status expressions
//

BOOST_DATA_TEST_CASE(
	status_and, bdata::xrange(3) ^ bdata::make({ fail, cont, done }), xr, anyStatus
){
	BOOST_TEST  ( [=] { task fail and anyStatus end }().failing() );
	BOOST_TEST  ( [=] { task cont and anyStatus end }().running() );
	BOOST_CHECK ( [=] { task done and anyStatus end }() == anyStatus );
}

BOOST_DATA_TEST_CASE(
	status_or, bdata::xrange(3) ^ bdata::make({ fail, cont, done }), xr, anyStatus
) {
	BOOST_CHECK( [=] { task fail or anyStatus end }() == anyStatus );
	BOOST_TEST ( [=] { task cont or anyStatus end }().running() );
	BOOST_TEST ( [=] { task done or anyStatus end }().complete() );
}

BOOST_DATA_TEST_CASE(
	status_over, bdata::xrange(3) ^ bdata::make({ fail, cont, done }), xr, anyStatus
) {
	BOOST_CHECK( [=] { task fail over anyStatus end }() == anyStatus );
	BOOST_TEST ( [=] { task cont over anyStatus end }().running()    );
	BOOST_CHECK( [=] { task done over anyStatus end }() == anyStatus );
}

//
// Subexpressions
//

BOOST_AUTO_TEST_CASE(status_subexpression) {
	BOOST_CHECK( [=] { 
		task 
			done
			and fail
			or done
		end 
	}() == done);
	//
	BOOST_CHECK([=] { 
		task
			done
		    and subtask(fail or done)
		end 
	}() == done);
}

status subexp   () { task done and subtask(fail or done) end }
status no_subexp() { task done and fail or done end }

BOOST_AUTO_TEST_CASE(subexp_perf_0) {
	for (int i = 0; i < 1000000; i++) no_subexp();
}

BOOST_AUTO_TEST_CASE(subexp_perf_1) {
	for (int i = 0; i < 1000000; i++) subexp();
}

//
// Parallel status expressions
//

BOOST_DATA_TEST_CASE(
	status_with, bdata::xrange(3) ^ bdata::make({ fail, cont, done }), xr, anyStatus
) {
	BOOST_TEST ( [=] { task fail with anyStatus end }().failing());
	BOOST_CHECK( [=] { task cont with anyStatus end }() == anyStatus);
	BOOST_TEST ( [=] { task done with anyStatus end }().complete());
}

BOOST_DATA_TEST_CASE(
	status_por, bdata::xrange(3) ^ bdata::make({ fail, cont, done }), xr, anyStatus
) {
	BOOST_CHECK( [=] { task fail por anyStatus end }() == anyStatus);
	BOOST_CHECK( [=] { task cont por anyStatus end }() == anyStatus);
	BOOST_TEST ( [=] { task done por anyStatus end }().complete());
}

BOOST_DATA_TEST_CASE(
	status_pand, bdata::xrange(3) ^ bdata::make({ fail, cont, done }), xr, anyStatus
) {
	BOOST_TEST ( [=] { task fail pand anyStatus end }().failing());
	BOOST_CHECK( [=] { task cont pand anyStatus end }() == anyStatus);
	BOOST_CHECK( [=] { task done pand anyStatus end }() == anyStatus);
}

//
// Status
//
BOOST_AUTO_TEST_CASE(status_constructor_no_arg) {
	BOOST_CHECK(status().running());
}

BOOST_AUTO_TEST_CASE(status_complete) {
	BOOST_CHECK(!status(-1).complete());
	BOOST_CHECK(!status(0).complete());
	BOOST_CHECK(status(1).complete());
}

BOOST_AUTO_TEST_CASE(status_running) {
	BOOST_CHECK(!status(-1).running());
	BOOST_CHECK(status(0).running());
	BOOST_CHECK(!status(1).running());
}

BOOST_AUTO_TEST_CASE(status_failing) {
	BOOST_CHECK(status(-1).failing());
	BOOST_CHECK(!status(0).failing());
	BOOST_CHECK(!status(1).failing());
}

BOOST_AUTO_TEST_CASE(status_impending) {
	BOOST_CHECK(status(-1).impending());
	BOOST_CHECK(status(0).impending());
	BOOST_CHECK(!status(1).impending());
}

BOOST_AUTO_TEST_CASE(status_immediate) {
	BOOST_CHECK( status(-1).immediate());
	BOOST_CHECK(!status(0).immediate());
	BOOST_CHECK( status(1).immediate());
}

BOOST_AUTO_TEST_CASE(status_pending) {
	BOOST_CHECK(!status(-1).pending());
	BOOST_CHECK(status(0).pending());
	BOOST_CHECK(status(1).pending());
}

BOOST_AUTO_TEST_CASE(status_equality) {
	BOOST_CHECK( fail == fail );
	BOOST_CHECK( cont == cont );
	BOOST_CHECK( done == done );
	//
	BOOST_CHECK(fail == status(-1));
	BOOST_CHECK(cont == status( 0));
	BOOST_CHECK(done == status(+1));
}

BOOST_AUTO_TEST_CASE(status_inequality) {
	BOOST_CHECK( fail != cont );
	BOOST_CHECK( fail != done );
	BOOST_CHECK( cont != fail );
	BOOST_CHECK( cont != done );
	BOOST_CHECK( done != fail );
	BOOST_CHECK( done != cont );
}

BOOST_AUTO_TEST_CASE(status_const_done) { BOOST_CHECK(done.complete()); }
BOOST_AUTO_TEST_CASE(status_const_fail) { BOOST_CHECK(fail.failing()); }
BOOST_AUTO_TEST_CASE(status_const_cont) { BOOST_CHECK(cont.running()); }