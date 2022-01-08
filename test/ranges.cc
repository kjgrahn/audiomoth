/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <ranges.h>

#include <sstream>

#include <orchis.h>

namespace ranges {

    using orchis::TC;
    using orchis::assert_eq;
    using orchis::assert_true;

    void assert_range(const Ranges& rr, const char* const s)
    {
	std::ostringstream oss;
	const char* prefix = "";
	for (auto r: rr) {
	    oss << prefix << r.a << '-' << r.b;
	    prefix = ", ";
	}
	assert_eq(oss.str(), s);
    }

    void empty(TC)
    {
	Ranges rr;
	assert_range(rr, "");
    }

    void single(TC)
    {
	Ranges rr;
	rr.add({5, 10});
	assert_range(rr, "5-10");
    }

    void singles(TC)
    {
	Ranges rr;
	rr.add({5, 10});
	rr.add({5, 10});
	rr.add({5, 10});
	assert_range(rr, "5-10");
    }

    void two(TC)
    {
	Ranges rr;
	rr.add({5, 10});
	assert_range(rr, "5-10");
	rr.add({1, 4});
	assert_range(rr, "1-4, 5-10");
    }

    void merge(TC)
    {
	Ranges rr;
	rr.add({5, 10});
	assert_range(rr, "5-10");
	rr.add({1, 4});
	assert_range(rr, "1-4, 5-10");
	rr.add({4, 5});
	assert_range(rr, "1-10");
    }

    void subrange(TC)
    {
	Ranges rr;
	rr.add({5, 10});
	rr.add({6,  9});
	rr.add({7,  8});
	assert_range(rr, "5-10");
    }

    void expand(TC)
    {
	Ranges rr;
	rr.add({5, 10});
	rr.add({6, 20});
	assert_range(rr, "5-20");
    }
}
