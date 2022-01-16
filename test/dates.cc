/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/dates.h>

#include <endians.h>

#include <string>
#include <vector>
#include <algorithm>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;

    void assert_encodes(const Dates& tt,
			time_t start,
			unsigned days)
    {
	orchis::assert_true(tt.valid());
	std::vector<uint8_t> v;
	tt.encode(std::back_inserter(v));

	auto p = begin(v);
	const unsigned a = le::get32(p);
	const unsigned b = le::get32(p);
	orchis::assert_true(p==end(v));

	assert_eq(a, start);
	assert_eq(a + days*24*3600, b);
    }

    void assert_invalid(const Dates& tt)
    {
	orchis::assert_false(tt.valid());
    }

    namespace dates {

	constexpr time_t jan16 = 1642291200;
	constexpr time_t jan31 = 1643587200;

	void empty(TC)
	{
	    const Dates tt;
	    assert_encodes(tt, 0, 0);
	}

	void day(TC)
	{
	    assert_encodes(Dates{"2022-01-16--2022-01-16"}, jan16, 1);
	    assert_encodes(Dates{"2022-01-31--2022-01-31"}, jan31, 1);
	}

	void week(TC)
	{
	    assert_encodes(Dates{"2022-01-16--2022-01-22"}, jan16, 7);
	}

	void year(TC)
	{
	    assert_encodes(Dates{"2022-01-16--2023-01-15"}, jan16, 365);
	}

	void plus(TC)
	{
	    assert_encodes(Dates{"2022-01-16+7"}, jan16, 7);
	}

	void invalid(TC)
	{
	    assert_invalid(Dates{""});
	    assert_invalid(Dates{"2022"});
	    assert_invalid(Dates{"2022-01"});
	    assert_invalid(Dates{"2022-01-16"});
	    assert_invalid(Dates{"2022-01-16--"});
	    assert_invalid(Dates{"2022-01-16--2022-02"});
	    assert_invalid(Dates{"2022-01-16-2022-01-16"});
	    assert_invalid(Dates{"2022-01-16+2022-01-16"});
	    assert_invalid(Dates{"food-be-bo"});
	    assert_invalid(Dates{"foobar"});

	    assert_invalid(Dates{"2022-01-16--2021-12-30"});
	}
    }
}
