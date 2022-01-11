/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_RANGES_H
#define AUDIOMOTH_RANGES_H

#include <vector>

/**
 * The range [a, b).  Note that b isn't included, so Range(5, 5) is an
 * empty range and there's a hole between Range(1, 2) and Range(3, 4).
 */
struct Range {
    Range() = default;
    Range(unsigned a, unsigned b) : a{a}, b{b} {}

    unsigned a = 0;
    unsigned b = 0;

    bool merge(const Range& other);
    Range fold(unsigned n);

    bool operator< (const Range& other) const;
    explicit operator bool () const { return a!=b; }
};

/**
 * Sorted, non-overlapping ranges of unsigned numbers, like this:
 *
 * ----#####----------###---##------------
 *
 * 1-2, 3-4 is the same thing as 3-4, 1-2, and you can fill holes:
 * adding 2-10 turns the range into 1-10 by covering the hole.
 *
 * Exists to support cfg::TimePeriods, but could perhaps be of general
 * interest.  The thing also appears in .newsrc files.
 */
class Ranges {
public:
    void add(Range r);

    auto size() const { return val.size(); }
    auto begin() const { return val.begin(); }
    auto end() const { return val.end(); }

private:
    std::vector<Range> val;
};

#endif
