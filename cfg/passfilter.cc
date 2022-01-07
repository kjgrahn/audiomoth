/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "passfilter.h"

#include "../split.h"

#include <cstdlib>

using cfg::PassFilter;

namespace {

    constexpr unsigned INVALID = ~0u;

    /**
     * Parse 'number [Hz | kHz]' and return rounded to unit 100 Hz:
     * 4.0 kHz encodes to 40.
     */
    unsigned hz(const char* s)
    {
	char* e;
	double n = std::strtod(s, &e);
	if (!n) return INVALID;
	while (*e==' ') e++;
	const std::string unit = e;
	if (unit=="" || unit=="Hz") {
	    ;
	}
	else if (unit=="kHz") {
	    n *= 1000;
	}
	else {
	    return INVALID;
	}
	return (n + 50) / 100;
    }

    unsigned hz(const std::string& s)
    {
	return hz(s.c_str());
    }
}

PassFilter::PassFilter(Kind kind, const std::string& s)
    : a {INVALID},
      b {INVALID}
{
    const auto v = split("-", s);

    if (kind==BAND) {
	if (v.size()!=2) return;
	a = hz(v[0]);
	b = hz(v[1]);
    }
    else {
	if (v.size()!=1) return;
	unsigned n = hz(v[0]);
	switch (kind) {
	case HIGH:
	    a = n;
	    b = INVALID;
	    break;
	case LOW:
	    a = INVALID;
	    b = n;
	    break;
	default:
	    break;
	}
    }
}

bool PassFilter::valid() const
{
    return a!=b || a==0;
}
