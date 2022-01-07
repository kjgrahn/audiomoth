/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_SAMPLERATE_H
#define AUDIOMOTH_CFG_SAMPLERATE_H

#include <string>
#include <array>
#include <iosfwd>

namespace cfg {

    /**
     * Sample rate, from 8 kHz to 384 kHz in steps. Default: 48 kHz.
     *
     * This encodes to 8 octets of sampler settings, so there's room
     * for a myriad more or less interesting settings. Interesting to
     * circuit benders, anyway. The ones supported here, though, are
     * the ones supported by the official configurator utility.
     */
    class SampleRate {
    public:
	SampleRate() = default;
	explicit SampleRate(const std::string& s);
	bool valid() const;

	template <class It>
	It encode(It p) const;

    private:
	uint8_t n = 3;
	std::array<uint8_t, 8> encode() const;
    };

    template <class It>
    It SampleRate::encode(It p) const
    {
	auto v = encode();
	return std::copy(begin(v), end(v), p);
    }
}

std::ostream& operator<< (std::ostream& os, const cfg::SampleRate& val);

#endif
