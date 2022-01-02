/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "device.h"
#include "utf8.h"

#include <hidapi/hidapi.h>

using hid::Device;

namespace {

    std::string err(hid_device* dev)
    {
	const wchar_t* s = hid_error(dev);
	if (!s) return "success";
	return utf8::encode(s);
    }
}

Device::Device(unsigned vendor, unsigned product)
    : val {hid_open(vendor, product, nullptr)}
{
    if (!val) error = err(nullptr);
}

Device::~Device()
{
    if (val) hid_close(val);
}

void Device::write(const hid::Packet& packet)
{
    int ret = hid_write(val, packet.data(), packet.size());
    if (ret==-1) {
	error = err(val);
	throw hid::Error {};
    }
    size_t n = ret;
    if (n != packet.size()) {
	error = "hid_write() underwrite";
	throw hid::Error {};
    }
}

hid::Packet Device::read(std::chrono::milliseconds timeout)
{
    hid::Packet packet;
    int ret = hid_read_timeout(val, packet.data(), packet.size(), timeout.count());
    if (ret==-1) {
	error = err(val);
	throw hid::Error {};
    }
    size_t n = ret;
    if (n != packet.size()) {
	error = "hid_read_timeout() underflow";
	throw hid::Error {};
    }

    return packet;
}
