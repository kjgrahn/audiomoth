/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/timeperiods.h>

#include <endians.h>

#include <string>
#include <vector>
#include <algorithm>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;
    using orchis::assert_true;

    void assert_encodes(const TimePeriods& tp,
			const unsigned len,
			const unsigned a0, const unsigned b0,
			const unsigned a1, const unsigned b1,
			const unsigned a2, const unsigned b2,
			const unsigned a3, const unsigned b3)
    {
	orchis::assert_true(tp.valid());
	std::vector<uint8_t> v;
	tp.encode(std::back_inserter(v));

	auto p = begin(v);
	assert_eq(le::get8(p), len);
	assert_eq(le::get16(p), a0); assert_eq(le::get16(p), b0);
	assert_eq(le::get16(p), a1); assert_eq(le::get16(p), b1);
	assert_eq(le::get16(p), a2); assert_eq(le::get16(p), b2);
	assert_eq(le::get16(p), a3); assert_eq(le::get16(p), b3);
	assert_true(p==end(v));
    }

    void add(TimePeriods& tp, const char* s)
    {
	assert_true(tp.add(s));
    }

    namespace timeperiods {

	void empty(TC)
	{
	    TimePeriods tp;
	    assert_encodes(tp, 0,
			   0, 0, 0, 0,
			   0, 0, 0, 0);
	}

	void simple(TC)
	{
	    TimePeriods tp;
	    add(tp, "05:30-07:10");
	    assert_encodes(tp, 1,
			   330, 430,
			   0, 0,
			   0, 0,
			   0, 0);
	}

	void duration(TC)
	{
	    TimePeriods tp;
	    add(tp, "05:30+01:40");
	    assert_encodes(tp, 1,
			   330, 430,
			   0, 0,
			   0, 0,
			   0, 0);
	}

	void duration2(TC)
	{
	    TimePeriods tp;
	    add(tp, "05:30+100");
	    assert_encodes(tp, 1,
			   330, 430,
			   0, 0,
			   0, 0,
			   0, 0);
	}

	void two(TC)
	{
	    TimePeriods tp;
	    add(tp, "16:00-19:30");
	    add(tp, "20:30-21:00");
	    assert_encodes(tp, 2,
			    960, 1170,
			   1230, 1260,
			   0, 0,
			   0, 0);
	}

	void sort(TC)
	{
	    TimePeriods tp;
	    add(tp, "20:30-21:00");
	    add(tp, "16:00-19:30");
	    assert_encodes(tp, 2,
			    960, 1170,
			   1230, 1260,
			   0, 0,
			   0, 0);
	}

	void merge(TC)
	{
	    TimePeriods tp;
	    add(tp, "16:00-19:30");
	    add(tp, "20:30-21:00");
	    assert_encodes(tp, 2,
			    960, 1170,
			   1230, 1260,
			   0, 0,
			   0, 0);
	    add(tp, "19:00-19:30");
	    add(tp, "20:30-20:40");
	    assert_encodes(tp, 2,
			    960, 1170,
			   1230, 1260,
			   0, 0,
			   0, 0);
	    add(tp, "19:00-20:00");
	    add(tp, "20:20-20:40");
	    assert_encodes(tp, 2,
			    960, 1200,
			   1220, 1260,
			   0, 0,
			   0, 0);
	    add(tp, "20:00-20:20");
	    assert_encodes(tp, 1,
			    960, 1260,
			   0, 0,
			   0, 0,
			   0, 0);
	}

	void full(TC)
	{
	    TimePeriods tp;
	    add(tp, "01:00+10");
	    add(tp, "04:00+10");
	    add(tp, "03:00+10");
	    add(tp, "02:00+10");
	    assert_encodes(tp, 4,
			    60,  70,
			   120, 130,
			   180, 190,
			   240, 250);
	}

	void midnight(TC)
	{
	    TimePeriods tp;
	    add(tp, "23:00-01:30");
	    assert_encodes(tp, 2,
			      0, 90,
			   1380, 1440,
			   0, 0,
			   0, 0);
	}
    }
}
