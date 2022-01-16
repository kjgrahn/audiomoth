/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/timezone.h>

#include <endians.h>

#include <string>
#include <vector>
#include <algorithm>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;

    void assert_encodes(const Timezone& tz,
			unsigned a, unsigned b)
    {
	orchis::assert_true(tz.valid());
	std::vector<uint8_t> v;
	tz.a().encode(std::back_inserter(v));
	tz.b().encode(std::back_inserter(v));

	auto p = begin(v);
	assert_eq(le::get8(p), a);
	assert_eq(le::get8(p), b);
	orchis::assert_true(p==end(v));
    }

    namespace timezone {

	void trivial(TC)
	{
	    const Timezone tz;
	    assert_encodes(tz, 0, 0);
	}
    }
}
