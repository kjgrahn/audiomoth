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
	    get8 ("--gain 0    ", p, 0x02);
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

	/* A configuration I pushed down to an 1.5.0 AudioMoth with
	 * the official 1.5.1 configurator AppImage and captured using
	 * strace(1). I had enabled a bunch of features so it wouldn't
	 * just be zero octets near the end.
	 *
	 * 00 06 16 fb f4 61 02 04 10 01 00 dc 05 00 08 05
	 * 00 37 00 01 02 3c 00 78 00 64 05 a0 05 00 00 00
	 * 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00
	 * 00 00 00 00 00 00 25 00 ff ff 00 00 02 00 00 03
	 *
	 */
	void reference(TC)
	{
	    Config cfg;
	    cfg.cyclic = Cyclic {"5+55"};
	    cfg.periods.add("01:00+60");
	    cfg.periods.add("23:00+60");
	    cfg.filter = PassFilter {PassFilter::HIGH, "3.7kHz"};
	    cfg.nimh_batteries = true;
	    cfg.save_energy = true;
	    cfg.elephants = true;

	    assert_true(cfg.valid());

	    std::array<uint8_t, 100> v;
	    v.fill(0xaa);
	    const auto end = cfg.encode(v.data());

	    auto p = v.data();
	    get8 ("--gain 0     ", p, 2);
	    get8 ("--rate 48kHz ", p, 4, 16, 1);
	    get32("             ", p, 384000);
	    get8 ("             ", p, 8);
	    get16("--cyclic 5+55", p, 5, 55);

	    get8 ("--no-rw-led  ", p, 1); // enableLED

	    get8 ("-t           ", p, 2);
	    get16("  1:00-2:00  ", p, 60, 120);
	    get16(" 23:00-24:00 ", p, 1380, 1440);
	    get16("             ", p, 0, 0);
	    get16("             ", p, 0, 0);
	    get16("             ", p, 0, 0);

	    get8 ("TZ (hours)  ", p, 0);
	    get8 ("--ignore-low", p, 0); // enableLowVoltageCutoff
	    get8 ("--no-battery", p, 0);
	    get8 ("TZ (minutes)", p, 0);

	    get8 ("--cyclic    ", p, 0); // disableSleepRecordCycle

	    get32("--dates     ", p, 0, 0);
	    get16("filter      ", p, 37, 0xffff);

	    get16("threshold A ", p, 0);
	    get8 ("pack #0     ", p, 2);
	    get8 ("pack #1     ", p, 0);
	    get8 ("pack #2     ", p, 0);
	    get8 ("pack #3     ", p, 3);

	    assert_eq(p, end);
	}
    }
}
