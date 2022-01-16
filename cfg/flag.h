/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_FLAG_H
#define AUDIOMOTH_CFG_FLAG_H

#include <iosfwd>

namespace cfg {

    /**
     * A boolean flag which ends up being encoded into some position
     * in a byte.
     */
    template <unsigned N>
    class Flag {
    public:
	Flag() = default;
	Flag(bool val) : val{val} {}
	bool valid() const { return true; }
	unsigned encode() const;
    private:
	bool val = false;
    };

    template <unsigned N> unsigned Flag<N>::encode() const
    {
	unsigned n = val;
	return n << N;
    }

    /**
     * Like Flag<N> but with the encoding inverted. "Enable battery
     * level indication" is like that, and I don't want to replace it
     * with "disable battery level indication."
     */
    template <unsigned N>
    class Xlag {
    public:
	Xlag() = default;
	Xlag(bool val) : val{val} {}
	bool valid() const { return true; }
	unsigned encode() const;
    private:
	bool val = false;
    };

    template <unsigned N> unsigned Xlag<N>::encode() const
    {
	unsigned n = !val;
	return n << N;
    }
}

template <unsigned N>
std::ostream& operator<< (std::ostream& os, const cfg::Flag<N>& val);

#endif
