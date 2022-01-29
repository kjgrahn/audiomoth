/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_TIMEPERIODS_H
#define AUDIOMOTH_CFG_TIMEPERIODS_H

#include "../ranges.h"

#include <string>
#include <array>
#include <iosfwd>

namespace cfg {

    /**
     * 0--4 time periods in the day, with 1 min resolution.
     *
     *   period period ...
     *
     * These are equivalent:
     *
     *   05:00-07:30
     *   05:00+02:30
     *
     * TBD what happens to timezones.
     */
    class TimePeriods {
    public:
	bool add(const std::string& s);
	bool valid() const;

	template <class It>
	It encode(It p) const;

    private:
	Ranges ranges;

	std::array<uint8_t, 21> encode() const;
    };

    template <class It>
    It TimePeriods::encode(It p) const
    {
	auto v = encode();
	return std::copy(begin(v), end(v), p);
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::TimePeriods& val);

#endif
