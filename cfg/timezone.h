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

	struct Hours {
	    template <class It> It encode(It p) const;
	};

	struct Minutes {
	    template <class It> It encode(It p) const;
	};

	Hours a() const { return {}; }
	Minutes b() const { return {}; }
    };

    template <class It> It Timezone::Hours::encode(It p) const
    {
	le::put8(p, 0);
	return p;
    }

    template <class It> It Timezone::Minutes::encode(It p) const
    {
	le::put8(p, 0);
	return p;
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::Timezone& val);

#endif
