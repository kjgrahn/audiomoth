/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/config.h>

#include <endians.h>

#include <string>
#include <array>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;
    using orchis::assert_true;

    template <class It>
    void get8(const char*, It& p, unsigned a)
    {
	assert_eq(le::get8(p), a);
    }

    template <class It>
    void get8(const char*, It& p, unsigned a, unsigned b, unsigned c)
    {
	assert_eq(le::get8(p), a);
	assert_eq(le::get8(p), b);
	assert_eq(le::get8(p), c);
    }

    template <class It>
    void get16(const char*, It& p, unsigned a)
    {
	assert_eq(le::get16(p), a);
    }

    template <class It>
    void get16(const char*, It& p, unsigned a, unsigned b)
    {
	assert_eq(le::get16(p), a);
	assert_eq(le::get16(p), b);
    }

    template <class It>
    void get32(const char*, It& p, unsigned a)
    {
	assert_eq(le::get32(p), a);
    }

    template <class It>
    void get32(const char*, It& p, unsigned a, unsigned b)
    {
	assert_eq(le::get32(p), a);
	assert_eq(le::get32(p), b);
    }

    namespace config {

	void simple(TC)
	{
	    Config cfg;
	    cfg.periods.add("22:56-23:30");

	    assert_true(cfg.valid());

	    std::array<uint8_t, 100> v;
	    v.fill(0xaa);
	    const auto end = cfg.encode(v.data());

	    auto p = v.data();
	    get8 ("--gain 0", p, 0x02);
	    get8 ("--rate 48kHz", p, 4, 16, 1);
	    get32("            ", p, 384000);
	    get8 ("            ", p, 8);
	    get16("--cyclic    ", p, 0, 0);

	    get8 ("--no-rw-led ", p, 1);

	    get8 ("-t          ", p, 1);
	    get16("            ", p, 1376, 1410);
	    get16("            ", p, 0, 0);
	    get16("            ", p, 0, 0);
	    get16("            ", p, 0, 0);

	    get8 ("TZ (hours)  ", p, 0);
	    get8 ("--ignore-low", p, 0);
	    get8 ("--no-battery", p, 0);
	    get8 ("TZ (minutes)", p, 0);

	    get8 ("--cyclic    ", p, 1);

	    get32("--dates     ", p, 0, 0);
	    get16("filter      ", p, 0, 0);

	    get16("threshold A ", p, 0);
	    get8 ("pack #0     ", p, 0);
	    get8 ("pack #1     ", p, 0);
	    get8 ("pack #2     ", p, 0);
	    get8 ("pack #3     ", p, 0);

	    assert_eq(p, end);
	}
    }
}
