/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <cfg/flag.h>

#include <orchis.h>

namespace cfg {

    using orchis::TC;
    using orchis::assert_eq;

    template <class F>
    void assert_encodes(const F f,
			unsigned n)
    {
	assert_eq(f.encode(), n);
    }

    namespace flag {

	void unset(TC)
	{
	    Flag<0> a;
	    Flag<1> b;
	    Flag<7> c;
	    assert_encodes(a, 0);
	    assert_encodes(b, 0);
	    assert_encodes(c, 0);
	}

	void cleared(TC)
	{
	    Flag<0> a = false;
	    Flag<1> b = false;
	    Flag<7> c = false;
	    assert_encodes(a, 0);
	    assert_encodes(b, 0);
	    assert_encodes(c, 0);
	}

	void set(TC)
	{
	    Flag<0> a = true;
	    Flag<1> b = true;
	    Flag<7> c = true;
	    assert_encodes(a, 0x01);
	    assert_encodes(b, 0x02);
	    assert_encodes(c, 0x80);
	}

	void xlag(TC)
	{
	    assert_encodes(Xlag<3>{},      0x08);
	    assert_encodes(Xlag<3>{false}, 0x08);
	    assert_encodes(Xlag<3>{true},  0);
	}
    }
}
