/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_CYCLIC_H
#define AUDIOMOTH_CFG_CYCLIC_H

#include "../endians.h"

#include <string>
#include <iosfwd>

namespace cfg {

    /**
     * Cyclic recording: either disabled, or sleeping at even intervals. For example:
     *
     *   50+10
     *
     * will record for 10 seconds every minute.
     *
     */
    class Cyclic {
    public:
	Cyclic() = default;
	explicit Cyclic(const std::string& s);
	bool valid() const;

	template <class It> It encode_a(It p) const;
	template <class It> It encode_b(It p) const;

    private:
	uint16_t sleep = 0;
	uint16_t record = 0;
    };

    template <class It> It Cyclic::encode_a(It p) const
    {
	le::put16(p, sleep);
	le::put16(p, record);
	return p;
    }

    template <class It> It Cyclic::encode_b(It p) const
    {
	bool enabled = sleep && record;
	le::put8(p, !enabled);
	return p;
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::Cyclic& val);

#endif
