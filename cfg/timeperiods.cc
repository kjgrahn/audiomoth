/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "timeperiods.h"

#include "../split.h"
#include "../endians.h"

#include <cstdlib>

using cfg::TimePeriods;

namespace {

    unsigned hhmm(const char* p)
    {
	char* end;
	unsigned n = std::strtoul(p, &end, 10);
	if (*end=='\0') return n;
	if (*end==':') {
	    p = end + 1;
	    n *= 60;
	    n += std::strtoul(p, &end, 10);
	    if (*end=='\0') return n;
	}
	return ~0u;
    }

    Range parse(const std::string& s)
    {
	bool dash = true;
	auto v = split("-", s);
	if (v.size() != 2) {
	    dash = false;
	    v = split("+", s);
	}
	if (v.size() != 2) return {};

	unsigned a = hhmm(v[0].c_str());
	unsigned b = hhmm(v[1].c_str());
	if (a==~0u) return {};
	if (b==~0u) return {};

	if (dash && b < a) b += 1440;
	if (!dash) b += a;
	return {a, b};
    }
}

/**
 * Add a time period, e.g. "05:00-07:30". Returns false on parse
 * error (but may not have very pedantic error checks).
 *
 * 05:00-07:30
 * 05:00+02:30
 * 05:00+150
 * 05:00-04:00  (23 hours)
 */
bool TimePeriods::add(const std::string& s)
{
    Range r = parse(s);
    if (!r) return false;

    Range t = r.fold(1440);
    if (t) ranges.add(t);

    ranges.add(r);
    return true;
}

bool TimePeriods::valid() const
{
    return ranges.size() <= 4;
}

std::array<uint8_t, 17> TimePeriods::encode() const
{
    std::array<uint8_t, 17> v;
    auto a = begin(v);
    const auto b = end(v);

    le::put8(a, ranges.size());
    for (const auto& r : ranges) {
	le::put16(a, r.a);
	le::put16(a, r.b);
    }
    while (a!=b) {
	le::put16(a, 0);
    }
    return v;
}
