/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_DATES_H
#define AUDIOMOTH_CFG_DATES_H

#include "../endians.h"

#include <string>
#include <iosfwd>

namespace cfg {

    /**
     * A range of dates when the device should record (or not,
     * depending on the rest of its schedule).
     *
     *   2022-01-16--2022-01-31    first day the 16th; last day the 31st
     *   2022-01-16+7              first day the 16th; record for 7 days
     *
     * Note that the end date is included.
     */
    class Dates {
    public:
	Dates() = default;
	explicit Dates(const std::string& s);
	bool valid() const;

	template <class It>
	It encode(It p) const;

    private:
	time_t a = 0;
	time_t b = 0;
    };

    template <class It>
    It Dates::encode(It p) const
    {
	le::put32(p, a);
	le::put32(p, b);
	return p;
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::Dates& val);

#endif
