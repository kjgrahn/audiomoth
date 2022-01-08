/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "ranges.h"

#include <algorithm>

/**
 * Merge 'other', or if that's not possible (there would be a hole in
 * the range) return false.
 *
 * Undefined result unless *this < other holds.
 */
bool Range::merge(const Range& other)
{
    if (b < other.a) return false;
    b = std::max(b, other.b);
    return true;
}

bool Range::operator< (const Range& other) const
{
    if (a==other.a) return b < other.b;
    return a < other.a;
}

namespace {

    /* From a sorted list of ranges, produce one without overlaps
     * (and thus possibly fewer but larger entries).
     *
     */
    template <class It>
    std::vector<Range> merge(It a, const It b)
    {
	std::vector<Range> res;
	if (a==b) return res;
	Range lr = *a++;
	while (a!=b) {
	    if (lr.merge(*a)) {
		a++;
	    }
	    else {
		res.push_back(lr);
		lr = *a++;
	    }
	}
	res.push_back(lr);
	return res;
    }
}

void Ranges::add(Range r)
{
    val.push_back(r);
    std::sort(val.begin(), val.end());
    val = merge(val.begin(), val.end());
}
