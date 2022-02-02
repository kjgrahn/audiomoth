/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_TRIGGER_H
#define AUDIOMOTH_CFG_TRIGGER_H

#include "../endians.h"

#include <string>
#include <iosfwd>

namespace cfg {

    /**
     * Acoustic trigger configuration: roughly how loud the sound
     * needs to be, and for how long it should keep the device
     * recording.
     *
     * This is four pieces of configuration which we encode in three
     * parts. Oddly, the device doesn't seem to use all of it: the
     * trigger in dB and the trigger in percentage are only used for
     * writing down in CONFIG.TXT on the SD card (and possibly as
     * metadata in the WAV files).
     *
     * For now, accept trigger config as a percentage, but pretend it
     * was given as a "raw 16-bit value". That way the device won't
     * interpret the percentage or dB fields and I can leave them
     * blank. And I don't have to reverse-engineer the transformations
     * back and forth between 0..1, 0--100% as mantissa and exponent,
     * decibels and whatnot.
     */
    class Trigger {
    public:
	Trigger() = default;
	bool amplitude(const std::string& s);
	bool duration(const std::string& s);

	bool valid() const;

	struct A {
	    const unsigned amplitude;
	    template <class It> It encode(It p) const;
	};
	A a() const { return A {amp}; }

	struct B {
	    const unsigned duration;
	    unsigned encode() const { return duration << 2; }
	};
	B b() const { return B {dur}; }

	struct C {
	    template <class It> It encode(It p) const;
	};
	C c() const { return {}; }

    private:
	unsigned amp = 0;
	unsigned dur = 0;
    };

    template <class It> It Trigger::A::encode(It p) const
    {
	le::put16(p, amplitude);
	return p;
    }

    template <class It> It Trigger::C::encode(It p) const
    {
	le::put16(p, 0);
	return p;
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::Trigger& val);

#endif
