/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "config.h"

namespace {

    template <class It, class C>
    void put(It& p, const C& c)
    {
	p = c.encode(p);
    }

    template <class It>
    void put(It& p,
	     cfg::Flag<0> c)
    {
	unsigned n = c.encode();
	le::put8(p, n);
    }

    template <class It>
    void put(It& p,
	     cfg::Xlag<0> c)
    {
	unsigned n = c.encode();
	le::put8(p, n);
    }

    template <class It, unsigned M, unsigned N>
    void put(It& p,
	     cfg::Flag<M> a,
	     cfg::Flag<N> b)
    {
	unsigned n = 0;
	n |= a.encode();
	n |= b.encode();
	le::put8(p, n);
    }

    template <class It, unsigned M, unsigned N>
    void put(It& p,
	     cfg::Flag<M> a,
	     cfg::Flag<N> b,
	     cfg::Trigger::B t)
    {
	unsigned n = 0;
	n |= a.encode();
	n |= b.encode();
	n |= t.encode();
	le::put8(p, n);
    }
}

template <class It>
It Config::encode(It p) const
{
    put(p, gain);
    put(p, samplerate);
    put(p, cyclic.a());
    put(p, rw_led);
    put(p, periods);
    put(p, tz.a());

    put(p, ignore_low_battery);
    put(p, battery_led);
    put(p, tz.b());
    put(p, cyclic.b());

    put(p, dates);
    put(p, filter);

    put(p, amplitude_triggered.a());

    put(p, need_chime,
	   nimh_batteries,
	   amplitude_triggered.b());

    put(p, amplitude_triggered.c());

    put(p, save_energy,
	   elephants);

    return p;
}

template uint8_t* Config::encode(uint8_t* p) const;

bool Config::valid() const
{
    bool v = true;
    v &= gain.valid();
    v &= samplerate.valid();
    v &= cyclic.valid();
    v &= rw_led.valid();
    v &= periods.valid();
    v &= tz.valid();
    v &= ignore_low_battery.valid();
    v &= battery_led.valid();
    v &= dates.valid();
    v &= filter.valid();
    v &= amplitude_triggered.valid();

    v &= need_chime.valid();
    v &= nimh_batteries.valid();
    v &= save_energy.valid();
    v &= elephants.valid();

    return v;
}
