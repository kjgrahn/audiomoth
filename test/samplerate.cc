/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/samplerate.h>

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

    void assert_encodes(const SampleRate& val,
			const unsigned d,
			const unsigned e)
    {
	orchis::assert_true(val.valid());
	std::vector<uint8_t> v;
	val.encode(std::back_inserter(v));

	auto p = begin(v);
	assert_eq(le::get8(p), 4);
	assert_eq(le::get8(p), 16);
	assert_eq(le::get8(p), 1);
	assert_eq(le::get32(p), d);
	assert_eq(le::get8(p), e);
	assert_true(p==end(v));
    }

    namespace samplerate {

	void empty(TC)
	{
	    const SampleRate val;
	    assert_encodes(val, 384000, 8);
	}

	void simple(TC)
	{
	    const SampleRate val {"48kHz"};
	    assert_encodes(val, 384000, 8);
	}

	void min(TC)
	{
	    const SampleRate val {"8kHz"};
	    assert_encodes(val, 384000, 48);
	}

	void max(TC)
	{
	    const SampleRate val {"384kHz"};
	    assert_encodes(val, 384000, 1);
	}

	void invalid(TC)
	{
	    const SampleRate val {"41kHz"};
	    assert_false(val.valid());
	}
    }
}
