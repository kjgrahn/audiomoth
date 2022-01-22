/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "cyclic.h"

#include "../split.h"

#include <vector>
#include <cstdlib>

using cfg::Cyclic;

namespace {

    constexpr uint16_t INVALID = ~0;

    unsigned seconds(const std::string& s)
    {
	const char* p = s.c_str();
	char* end;
	unsigned n = std::strtoul(p, &end, 10);
	if (*end) n = INVALID;
	if (!n) n = INVALID;
	return n;
    }
}

Cyclic::Cyclic(const std::string& s)
    : sleep {INVALID},
      record {INVALID}
{
    const auto v = split("+", s);
    if (v.size()==2) {
	sleep = seconds(v[0]);
	record = seconds(v[1]);
    }
}

bool Cyclic::valid() const
{
    return (sleep | record) != INVALID;
}

Cyclic::Values::Values(uint16_t sleep, uint16_t record)
    : sleep {sleep},
      record {record}
{}

Cyclic::Enabled::Enabled(uint16_t sleep, uint16_t record)
    : sleep {sleep},
      record {record}
{}
