/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "dates.h"

#include <array>
#include <algorithm>
#include <cctype>
#include <cstring>

#include <time.h>

using cfg::Dates;

namespace {

    template <class It>
    bool match(It a, const It b, const char* p)
    {
	const size_t n = std::distance(a, b);
	if (n != std::strlen(p)) return false;
	while (a!=b) {
	    if (*p=='n') {
		if (!std::isdigit(static_cast<unsigned char>(*a))) return false;
	    }
	    else {
		if (*p != *a) return false;
	    }
	    p++;
	    a++;
	}
	return true;
    }

    bool match(const std::string& s,
	       const char* p)
    {
	return match(begin(s), end(s), p);
    }

    bool match(const std::string& s,
	       const char* p,
	       const char* q,
	       const char* r)
    {
	return match(s, p) || match(s, q) || match(s, r);
    }

    /* Consume a decimal number, advance p past it, and advance some
     * steps more. Obviously you need to know the overall format of
     * the string already.
     */
    unsigned eatn(const char*& p, unsigned suffix = 0)
    {
	char* end;
	unsigned n = std::strtoul(p, &end, 10);
	p = end + suffix;
	return n;
    }

    /* Return the time_t at the UTC midnight preceding yyyy-mm-dd.
     */
    time_t midnight(unsigned yyyy, unsigned mm, unsigned dd)
    {
	struct tm tm {};
	tm.tm_year = yyyy-1900;
	tm.tm_mon  = mm - 1;
	tm.tm_mday = dd;
	return timegm(&tm);
    }
}

Dates::Dates(const std::string& s)
{
    const char* p = s.c_str();
    if (match(s, "nnnn-nn-nn--nnnn-nn-nn")) {

	{   unsigned yyyy = eatn(p, 1);
	    unsigned mm   = eatn(p, 1);
	    unsigned dd   = eatn(p, 2);
	    a = midnight(yyyy, mm, dd);
	}
	{   unsigned yyyy = eatn(p, 1);
	    unsigned mm   = eatn(p, 1);
	    unsigned dd   = eatn(p, 2);
	    b = midnight(yyyy, mm, dd+1);
	}
    }
    else if (match(s, "nnnn-nn-nn+n",
		      "nnnn-nn-nn+nn",
		      "nnnn-nn-nn+nnn")) {
	unsigned yyyy = eatn(p, 1);
	unsigned mm   = eatn(p, 1);
	unsigned dd   = eatn(p, 1);
	a = midnight(yyyy, mm, dd);
	b = a + 3600 * 24 * eatn(p);
    }
    else {
	a = 1;
	b = 0;
    }
}

bool Dates::valid() const
{
    return a <= b;
}
