/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_ENDIAN_H
#define AUDIOMOTH_ENDIAN_H

#include <cstdint>

namespace be {

    template <class It>
    void put8(It& p, unsigned n)
    {
	*p++ = n;
    }

    template <class It>
    void put16(It& p, unsigned n)
    {
	put8(p, n>>8);
	put8(p, n);
    }

    template <class It>
    void put32(It& p, unsigned n)
    {
	put16(p, n>>16);
	put16(p, n);
    }
}

namespace le {

    template <class It>
    unsigned get8(It& p)
    {
	return *p++;
    }

    template <class It>
    unsigned get16(It& p)
    {
	unsigned n = get8(p);
	n |= (get8(p) << 8);
	return n;
    }

    template <class It>
    unsigned get32(It& p)
    {
	unsigned n = get16(p);
	n |= (get16(p) << 16);
	return n;
    }

    template <class It>
    void put8(It& p, unsigned n)
    {
	*p++ = n;
    }

    template <class It>
    void put16(It& p, unsigned n)
    {
	put8(p, n);
	put8(p, n>>8);
    }

    template <class It>
    void put32(It& p, unsigned n)
    {
	put16(p, n);
	put16(p, n>>16);
    }
}

#endif
