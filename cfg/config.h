/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_CFG_CONFIG_H
#define AUDIOMOTH_CFG_CONFIG_H

#include "gain.h"
#include "samplerate.h"
#include "cyclic.h"
#include "passfilter.h"
#include "timeperiods.h"
#include "timezone.h"
#include "dates.h"
#include "trigger.h"
#include "flag.h"

/**
 * Device configuration, with suitable defaults, which can be modified
 * and finally sent to the device as a SET_APP_PACKET.
 *
 * Configuration items/options appear below roughly in the same order
 * as encoded into the packet, but some of them are spread out.
 */
struct Config {

    cfg::Gain gain;
    cfg::SampleRate samplerate;
    cfg::Cyclic cyclic;
    cfg::Flag<0> rw_led = true;
    cfg::TimePeriods periods;
    cfg::Timezone tz;
    cfg::Flag<0> ignore_low_battery = false;
    cfg::Xlag<0> battery_led = true;
    cfg::Dates dates;
    cfg::PassFilter filter;
    cfg::Trigger amplitude_triggered;

    cfg::Flag<0> need_chime = false;
    cfg::Flag<1> nimh_batteries = false;
    cfg::Flag<0> save_energy = false;
    cfg::Flag<1> elephants = false;

    bool valid() const;

    template <class It>
    It encode(It p) const;
};

#endif
