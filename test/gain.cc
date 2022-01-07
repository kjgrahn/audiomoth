/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/gain.h>

#include <endians.h>

#include <string>
#include <vector>
#include <algorithm>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;

    void assert_encodes(const Gain& g,
			const unsigned a)
    {
	orchis::assert_true(g.valid());
	std::vector<uint8_t> v;
	g.encode(std::back_inserter(v));

	assert_eq(v.size(), 1);
	assert_eq(v.front(), a);
    }

    void assert_invalid(const Gain& g)
    {
	orchis::assert_false(g.valid());
    }

    namespace gain {

	void empty(TC)
	{
	    const Gain g;
	    assert_encodes(g, 2);
	}

	void simple(TC)
	{
	    assert_encodes(Gain{"-2"}, 0);
	    assert_encodes(Gain{"-1"}, 1);
	    assert_encodes(Gain{ "0"}, 2);
	    assert_encodes(Gain{"+1"}, 3);
	    assert_encodes(Gain{"+2"}, 4);
	}

	void invalid(TC)
	{
	    assert_invalid(Gain{""});
	    assert_invalid(Gain{"+3"});
	    assert_invalid(Gain{"-3"});
	    assert_invalid(Gain{"2"});
	}

	void spinal_tap(TC)
	{
	    assert_invalid(Gain{"+11"});
	}
    }
}
