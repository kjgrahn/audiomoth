/* -*- c++ -*-
 *
 * Copyright (c) 2022 J�rgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_PACKET_H
#define AUDIOMOTH_PACKET_H

#include <array>
#include <cstdint>

namespace hid {

    using Packet = std::array<uint8_t, 64>;
}

#endif
