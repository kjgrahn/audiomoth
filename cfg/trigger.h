/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_TRIGGER_H
#define AUDIOMOTH_CFG_TRIGGER_H

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

	struct A {
	    template <class It> It encode(It p) const;
	};
	A a() const;

	struct B {
	    unsigned encode() const;
	};
	B b() const;

	struct C {
	    template <class It> It encode(It p) const;
	};
	C c() const;
    };
}

std::ostream& operator<< (std::ostream& os, const cfg::Trigger& val);

#endif
