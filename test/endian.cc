/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <endians.h>

#include <orchis.h>
#include <string>

namespace endian {

    using orchis::TC;

    namespace big {

	void test8(TC)
	{
	    std::string s {"...."};
	    auto it = begin(s);
	    be::put8(it, 0x55);
	    be::put8(it, 0x6e);
	    be::put8(it, 0x69);
	    be::put8(it, 0x78);
	    orchis::assert_eq(s, "Unix");
	}

	void test16(TC)
	{
	    std::string s {"...."};
	    auto it = begin(s);
	    be::put16(it, 0x556e);
	    be::put16(it, 0x6978);
	    orchis::assert_eq(s, "Unix");
	}

	void test32(TC)
	{
	    std::string s {"...."};
	    auto it = begin(s);
	    be::put32(it, 0x556e6978);
	    orchis::assert_eq(s, "Unix");
	}
    }

    namespace little {

	using orchis::assert_eq;

	namespace get {

	    void test8(TC)
	    {
		const std::string s {"Unix"};
		auto it = begin(s);
		assert_eq(le::get8(it), 0x55);
		assert_eq(le::get8(it), 0x6e);
		assert_eq(le::get8(it), 0x69);
		assert_eq(le::get8(it), 0x78);
	    }

	    void test16(TC)
	    {
		const std::string s {"Unix"};
		auto it = begin(s);
		assert_eq(le::get16(it), 0x6e55);
		assert_eq(le::get16(it), 0x7869);
	    }

	    void test32(TC)
	    {
		const std::string s {"Unix"};
		auto it = begin(s);
		assert_eq(le::get32(it), 0x78696e55);
	    }

	    void test64(TC)
	    {
		const unsigned v[] = { 1, 2, 3, 4,
				       5, 6,
				       7, 8 };
		auto it = std::begin(v);
		assert_eq(le::get32(it), 0x04030201);
		assert_eq(le::get16(it), 0x0605);
		assert_eq(le::get16(it), 0x0807);
	    }
	}

	namespace put {

	    void test8(TC)
	    {
		std::string s {"...."};
		auto it = begin(s);
		le::put8(it, 0x55);
		le::put8(it, 0x6e);
		le::put8(it, 0x69);
		le::put8(it, 0x78);
		assert_eq(s, "Unix");
	    }

	    void test16(TC)
	    {
		std::string s {"...."};
		auto it = begin(s);
		le::put16(it, 0x6e55);
		le::put16(it, 0x7869);
		assert_eq(s, "Unix");
	    }

	    void test32(TC)
	    {
		std::string s {"...."};
		auto it = begin(s);
		le::put32(it, 0x78696e55);
		assert_eq(s, "Unix");
	    }
	}
    }
}
