/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <utf8.h>

#include <orchis.h>

namespace utf8 {

    using orchis::TC;

    void assert_encodes(const wchar_t* s, const char* ref)
    {
	orchis::assert_eq(encode(s), ref);
    }

    void empty(TC)
    {
	assert_encodes(L"", "");
    }

    void ascii(TC)
    {
	assert_encodes(L"foo", "foo");
	assert_encodes(L"Foo\nFoo\nFoo\nFoo\nFoo\nFoo\nFoo\nFoo\n",
		       "Foo\nFoo\nFoo\nFoo\nFoo\nFoo\nFoo\nFoo\n");
    }

    void utf8(TC)
    {
	assert_encodes(L"r\u00e4va", "r\xc3\xa4va");
    }

    void dash(TC)
    {
	assert_encodes(L"a\u2013b", "a\xe2\x80\x93" "b");
    }

    void skunk(TC)
    {
	assert_encodes(L"skunk:\U0001f9a8.", "skunk:\xf0\x9f\xa6\xa8.");
    }
}
