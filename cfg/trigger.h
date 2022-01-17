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
     * We don't support configuring this yet, so this class is just
     * used to pad out the configuration message.
     */
    class Trigger {
    public:
	bool valid() const { return true; }

	struct A {
	    template <class It> It encode(It p) const;
	};
	A a() const { return {}; }

	struct B {
	    unsigned encode() const { return 0; }
	};
	B b() const { return {}; }

	struct C {
	    template <class It> It encode(It p) const;
	};
	C c() const { return {}; }
    };

    template <class It> It Trigger::A::encode(It p) const
    {
	le::put16(p, 0);
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
