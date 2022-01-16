/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_TIMEZONE_H
#define AUDIOMOTH_CFG_TIMEZONE_H

#include "../endians.h"

#include <string>
#include <iosfwd>

namespace cfg {

    /**
     * Timezone configuration.  Currently hardcoded to UTC.
     *
     */
    class Timezone {
    public:
	bool valid() const { return true; }

	template <class It> It encode_a(It p) const;
	template <class It> It encode_b(It p) const;
    };

    template <class It> It Timezone::encode_a(It p) const
    {
	le::put8(p, 0);
	return p;
    }

    template <class It> It Timezone::encode_b(It p) const
    {
	le::put8(p, 0);
	return p;
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::Timezone& val);

#endif
