/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "gain.h"

#include <array>
#include <algorithm>

using cfg::Gain;

namespace {

    constexpr std::array<const char*, 5> names {"-2", "-1", "0", "+1", "+2"};
}

Gain::Gain(const std::string& s)
{
    auto p = std::find(begin(names), end(names), s);
    if (p==end(names)) {
	val = ~0;
    }
    val = p - begin(names);
}

bool Gain::valid() const
{
    return val < 5;
}
