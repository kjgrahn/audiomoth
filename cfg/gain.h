/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_GAIN_H
#define AUDIOMOTH_CFG_GAIN_H

#include <string>
#include <iosfwd>

namespace cfg {

    /**
     * Gain, one of five settings:
     *
     *   -2 low
     *   -1
     *    0 medium, default
     *   +1
     *   +2 high
     */
    class Gain {
    public:
	Gain() = default;
	explicit Gain(const std::string& s);
	bool valid() const;

	template <class It>
	It encode(It p) const;

    private:
	uint8_t val = 2;
    };

    template <class It>
    It Gain::encode(It p) const
    {
	*p++ = val;
	return p;
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::Gain& val);

#endif
