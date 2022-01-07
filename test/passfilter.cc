/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/passfilter.h>

#include <endians.h>

#include <string>
#include <vector>
#include <algorithm>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;

    void assert_encodes(const PassFilter& f,
			const unsigned a, const unsigned b)
    {
	orchis::assert_true(f.valid());
	std::vector<uint8_t> v;
	f.encode(std::back_inserter(v));

	auto p = begin(v);
	assert_eq(le::get16(p), a);
	assert_eq(le::get16(p), b);
	orchis::assert_true(p==end(v));
    }

    void assert_invalid(const PassFilter& f)
    {
	orchis::assert_false(f.valid());
    }

    namespace passfilter {

	void empty(TC)
	{
	    const PassFilter f;
	    assert_encodes(f, 0, 0);
	}

	void high(TC)
	{
	    const auto HIGH = PassFilter::HIGH;
	    assert_encodes({HIGH, "60kHz"},    600, 0xffff);
	    assert_encodes({HIGH, "60 kHz"},   600, 0xffff);
	    assert_encodes({HIGH, "60.0 kHz"}, 600, 0xffff);
	    assert_encodes({HIGH, "60000Hz"},  600, 0xffff);
	    assert_encodes({HIGH, "60000 Hz"}, 600, 0xffff);
	    assert_encodes({HIGH, "60000"},    600, 0xffff);
	    assert_encodes({HIGH, "59960 Hz"}, 600, 0xffff);
	    assert_encodes({HIGH, "59.96kHz"}, 600, 0xffff);
	}

	void low(TC)
	{
	    const auto LOW = PassFilter::LOW;
	    assert_encodes({LOW, "60kHz"},    0xffff, 600);
	    assert_encodes({LOW, "60 kHz"},   0xffff, 600);
	    assert_encodes({LOW, "60.0 kHz"}, 0xffff, 600);
	    assert_encodes({LOW, "60000Hz"},  0xffff, 600);
	    assert_encodes({LOW, "60000 Hz"}, 0xffff, 600);
	    assert_encodes({LOW, "60000"},    0xffff, 600);
	    assert_encodes({LOW, "59960 Hz"}, 0xffff, 600);
	    assert_encodes({LOW, "59.96kHz"}, 0xffff, 600);
	}

	void band(TC)
	{
	    const auto BAND = PassFilter::BAND;
	    assert_encodes({BAND, "6kHz-8kHz"},     60, 80);
	    assert_encodes({BAND, "6.1kHz-8.1kHz"}, 61, 81);
	}

	void invalid(TC)
	{
	    assert_invalid({PassFilter::LOW, ""});
	    assert_invalid({PassFilter::LOW, "foo"});
	    assert_invalid({PassFilter::BAND, "666Hz"});
	    assert_invalid({PassFilter::BAND, "1MHz"});
	}

	void invalid2(TC)
	{
	    assert_invalid({PassFilter::BAND, "666Hz-"});
	    assert_invalid({PassFilter::BAND, "-666Hz"});
	}
    }
}
