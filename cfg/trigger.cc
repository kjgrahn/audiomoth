/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "trigger.h"

#include <array>
#include <algorithm>

#include <cstdlib>

using cfg::Trigger;

namespace {

    /* Generate what the official software calls VALID_AMPLITUDE_VALUES.
     * It might be a hardware thing; the sampler may be limited in
     * which amplitude values it can use as thresholds.  None of these
     * threshold values have more than four bits set; they are all a
     * nybble and an offset.
     *
     * It's also logarithmic, or exponential, or something. If you
     * plot it lin-log, you get a fairly linear curve.
     */
    std::array<uint16_t, 98> valid_amplitudes()
    {
	std::array<uint16_t, 98> v;
	auto p = begin(v);

	for (unsigned a : {0,  1,  2,  4,  6,
			   8, 10, 12, 14, 16}) {
	    *p++ = a;
	}

	auto f = [&p] (unsigned n) {
	    for (unsigned a: {9, 10, 11, 12, 13, 14, 15, 16}) {
		*p++ = a << n;
	    }
	};

	f(1); f(2); f(3); f(4);
	f(5); f(6); f(7); f(8);
	f(9); f(10); f(11);
	return v;
    }
}

/**
 * Amplitude, as 0--100%. This chooses, linearly, one of the "valid
 * amplitudes".  Presumably this ends up making sense to a human ear:
 * so 50% is twice as loud as 25%, or something.  It won't be worse
 * than the official software's slider UI, anyway.
 */
bool Trigger::amplitude(const std::string& s)
{
    const char* const p = s.c_str();
    char* end;
    size_t n = std::strtoul(p, &end, 10);
    if (end==p) return false;
    if (std::string{end} != "%") return false;
    if (n > 100) return false;

    const auto v = valid_amplitudes();

    /* We should find the closest valid amplitude, but since they are
     * 98, we can pretend they are 100 (or 101, rather) and get away
     * with it. It's not as if anyone is going to notice; choosing a
     * threshold must be an ad-hoc activity anyway.
     */
    n = std::min(n, v.size()-1);
    amp = v[n];
    if (dur==0) dur = ~0u;
    return true;
}

/**
 * Duration, as 0--60 seconds. The official software only lets you
 * choose from [0, 1, 2, 5, 10, 15, 30, 60] but that's probably an UI
 * issue. I could probably get away with 0--63 since it's encoded to
 * six bits, but it's too hard to explain and 60 seconds is a nice and
 * even number.
 */
bool Trigger::duration(const std::string& s)
{
    const char* const p = s.c_str();
    char* end;
    unsigned n = std::strtoul(p, &end, 10);
    if (end==p) return false;
    if (*end) return false;
    if (n > 60) return false;
    dur = n;
    if (amp==0) amp = ~0u;
    return true;
}

bool Trigger::valid() const
{
    if (amp==~0u) return false;
    if (dur==~0u) return false;
    return true;
}
