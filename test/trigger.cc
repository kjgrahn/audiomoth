/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/trigger.h>

#include <endians.h>

#include <string>
#include <vector>
#include <algorithm>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;
    using orchis::assert_true;
    using orchis::assert_false;

    void assert_encodes(const Trigger& val,
			const unsigned a,
			const unsigned b,
			const unsigned ca = 0,
			const unsigned cb = 0)
    {
	assert_true(val.valid());
	std::vector<uint8_t> v;
	val.a().encode(std::back_inserter(v));
	val.c().encode(std::back_inserter(v));

	auto p = begin(v);
	assert_eq(le::get16(p), a);
	assert_eq(le::get8 (p), ca);
	assert_eq(le::get8 (p), cb);
	assert_true(p==end(v));

	assert_eq(val.b().encode(), b);
    }

    void assert_invalid(const Trigger& val)
    {
	orchis::assert_false(val.valid());
    }

    namespace trigger {

	void empty(TC)
	{
	    const Trigger t;
	    assert_encodes(t, 0, 0);
	}

	void max(TC)
	{
	    Trigger t;
	    assert_true(t.amplitude("100%"));
	    assert_true(t.duration("30"));
	    assert_encodes(t, 32768, 30 << 2);
	}

	void half(TC)
	{
	    Trigger t;
	    assert_true(t.amplitude("50%"));
	    assert_true(t.duration("10"));
	    assert_encodes(t, 576, 10 << 2);
	}

	void invalid(TC)
	{
	    Trigger t;
	    assert_true(t.amplitude("50%"));
	    assert_invalid(t);

	    t = {};
	    assert_true(t.duration("10"));
	    assert_invalid(t);

	    assert_true(t.amplitude("50%"));
	    assert_encodes(t, 576, 10 << 2);
	}

	void amplitude(TC)
	{
	    Trigger t;

	    assert_true(t.amplitude("50%"));
	    assert_true(t.amplitude("0%"));
	    assert_true(t.amplitude("99%"));
	    assert_true(t.amplitude("100%"));

	    assert_false(t.amplitude("50"));
	    assert_false(t.amplitude("50 %"));
	    assert_false(t.amplitude("50%%"));
	    assert_false(t.amplitude("foo%"));
	    assert_false(t.amplitude("%"));
	}

	void duration(TC)
	{
	    Trigger t;

	    assert_true(t.duration("0"));
	    assert_true(t.duration("1"));
	    assert_true(t.duration("10"));
	    assert_true(t.duration("59"));
	    assert_true(t.duration("60"));

	    assert_false(t.duration(""));
	    assert_false(t.duration("61"));
	    assert_false(t.duration("foo"));
	    assert_false(t.duration("10%"));
	}
    }
}
