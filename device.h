/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_DEVICE_H
#define AUDIOMOTH_DEVICE_H

#include "packet.h"

#include <string>
#include <chrono>

typedef struct hid_device_ hid_device;

namespace hid {

    struct Error {};

    /**
     * Wrapper for struct hid_device_, the file-like thingy in hidapi.
     * Just the parts interesting to us.
     */
    class Device {
    public:
	Device(unsigned vendor, unsigned product);
	~Device();
	Device(const Device&) = delete;
	Device& operator= (const Device&) = delete;

	explicit operator bool() const { return val; }

	void write(const Packet& packet);
	Packet read(std::chrono::milliseconds timeout);
	std::string error;

	hid_device* const val;
    };
}

/**
 * Typed interface. Write a Msg and read the matching response. Msg
 * and its response must serialise to and deserialise from Packets.
 *
 * May throw hid::Error, at which point dev.error is hopefully set.
 * May throw rx::Error, which is more about bad response data, and
 * doesn't carry any more detail.
 */
template <class Msg>
typename Msg::response_t write(hid::Device& dev, const Msg& msg)
{
    const auto p = msg.encode();
    dev.write(p);
    const auto r = dev.read(std::chrono::milliseconds{100});
    return typename Msg::response_t {msg.tag, r};
}

#endif
