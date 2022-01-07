/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "samplerate.h"

#include "../endians.h"

#include <array>
#include <algorithm>

using cfg::SampleRate;

namespace {

    constexpr std::array<const char*, 16> names {
	  "8",   "8kHz",
	 "16",  "16kHz",
	 "32",  "32kHz",
	 "48",  "48kHz",
	 "96",  "96kHz",
	"192", "192kHz",
	"250", "250kHz",
	"384", "384kHz",
    };

    struct Setting {
	unsigned char clockDivider;
	unsigned char acquisitionCycles;
	unsigned char oversampleRate;
	unsigned      sampleRate;
	unsigned char sampleRateDivider;
    };

    constexpr std::array<Setting, 8> settings = {
	Setting { 4, 16, 1, 384000, 48 },
	Setting { 4, 16, 1, 384000, 24 },
	Setting { 4, 16, 1, 384000, 12 },
	Setting { 4, 16, 1, 384000,  8 },
	Setting { 4, 16, 1, 384000,  4 },
	Setting { 4, 16, 1, 384000,  2 },
	Setting { 4, 16, 1, 250000,  1 },
	Setting { 4, 16, 1, 384000,  1 },
    };
}

SampleRate::SampleRate(const std::string& s)
{
    auto p = std::find(begin(names), end(names), s);
    if (p==end(names)) {
	n = ~0;
    }
    n = (p - begin(names)) / 2;
}

bool SampleRate::valid() const
{
    return n < 8;
}

std::array<uint8_t, 8> SampleRate::encode() const
{
    std::array<uint8_t, 8> v;
    auto a = begin(v);

    const Setting& s = settings[n];
    le::put8 (a, s.clockDivider);
    le::put8 (a, s.acquisitionCycles);
    le::put8 (a, s.oversampleRate);
    le::put32(a, s.sampleRate);
    le::put8 (a, s.sampleRateDivider);
    return v;
}
