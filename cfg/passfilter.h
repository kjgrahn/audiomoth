/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_PASSFILTER_H
#define AUDIOMOTH_CFG_PASSFILTER_H

#include "../endians.h"

#include <string>
#include <iosfwd>

namespace cfg {

    /**
     * High/low/band pass filter settings.  For example:
     *
     *   HIGH 60.0kHz      high-pass; 60 kHz and higher pass through
     *   LOW  20.0kHz      low-pass; 20 kHz and lower
     *   BAND 100Hz-20kHz  band-pass
     *
     * By default, disabled.
     */
    class PassFilter {
    public:
	enum Kind { NONE, HIGH, LOW, BAND };

	PassFilter() = default;
	PassFilter(Kind kind, const std::string& s);
	bool valid() const;

	template <class It>
	It encode(It p) const;

    private:
	unsigned a = 0;
	unsigned b = 0;
    };

    template <class It>
    It PassFilter::encode(It p) const
    {
	le::put16(p, a);
	le::put16(p, b);
	return p;
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::PassFilter& val);

#endif
