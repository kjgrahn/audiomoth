/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/cyclic.h>

#include <endians.h>

#include <string>
#include <vector>
#include <algorithm>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;

    void assert_encodes(const Cyclic& c,
			unsigned sleep, unsigned record,
			bool enable)
    {
	orchis::assert_true(c.valid());
	std::vector<uint8_t> v;
	c.encode_a(std::back_inserter(v));
	c.encode_b(std::back_inserter(v));

	auto p = begin(v);
	assert_eq(le::get16(p), sleep);
	assert_eq(le::get16(p), record);
	assert_eq(le::get8(p),  enable);
	orchis::assert_true(p==end(v));
    }

    void assert_invalid(const Cyclic& c)
    {
	orchis::assert_false(c.valid());
    }

    namespace cyclic {

	void empty(TC)
	{
	    const Cyclic c;
	    assert_encodes(c, 0, 0, true);
	}

	void simple(TC)
	{
	    assert_encodes(Cyclic{"1+2"},      1,  2, false);
	    assert_encodes(Cyclic{"300+60"}, 300, 60, false);
	}

	void invalid(TC)
	{
	    assert_invalid(Cyclic{""});
	    assert_invalid(Cyclic{"a+2"});
	    assert_invalid(Cyclic{"2+a"});
	}
    }
}
