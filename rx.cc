/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "message.h"
#include "endians.h"
#include "put_time.h"
#include "hexdump.h"

#include <algorithm>
#include <cstdio>
#include <iostream>

rx::Time::Time(const unsigned tag,
	       const hid::Packet& packet)
{
    auto p = begin(packet);
    if (le::get8(p) != tag) throw Error {};
    val = le::get32(p);
}

std::ostream& operator<< (std::ostream& os, const rx::Time& val)
{
    return put_time(os, val.val);
}

rx::Uid::Uid(const unsigned tag,
	     const hid::Packet& packet)
{
    auto p = begin(packet);
    if (le::get8(p) != tag) throw Error {};
    std::reverse_copy(p, p+8, begin(val));
}

std::ostream& operator<< (std::ostream& os, const rx::Uid& val)
{
    const char* prefix="";
    auto p = begin(val.val);
    while (p != end(val.val)) {
	uint8_t c = *p++;
	uint8_t d = *p++;
	char buf[6];
	std::snprintf(buf, sizeof buf, "%s%02hhx%02hhx", prefix, c, d);
	prefix=":";
	os << buf;
    }
    return os;
}

rx::Battery::Battery(const unsigned tag,
		     const hid::Packet& packet)
{
    auto p = begin(packet);
    if (le::get8(p) != tag) throw Error {};
    val = le::get8(p);
}

std::ostream& operator<< (std::ostream& os, const rx::Battery& val)
{
    if (val.val==0x0) return os << "< 3.6V";
    if (val.val==0xf) return os << "> 4.9V";
    const double n = 3.5 + val.val/10.0;
    char buf[5];
    std::snprintf(buf, sizeof buf, "%.1fV", n);
    return os << buf;
}

rx::Version::Version(const unsigned tag,
		     const hid::Packet& packet)
{
    auto p = begin(packet);
    if (le::get8(p) != tag) throw Error {};
    std::copy(p, p+3, begin(val));
}

std::ostream& operator<< (std::ostream& os, const rx::Version& val)
{
    auto v = val.val;
    return os << v[0] << '.'
	      << v[1] << '.'
	      << v[2];
}

rx::Description::Description(const unsigned tag,
			     const hid::Packet& packet)
{
    auto p = begin(packet);
    if (le::get8(p) != tag) throw Error {};
    auto e = std::find(p, end(packet), 0);
    val = {p, e};
}

std::ostream& operator<< (std::ostream& os, const rx::Description& val)
{
    return os << val.val;
}

rx::Packet::Packet(const unsigned tag,
		   const hid::Packet& packet)
    : val {packet}
{
    auto p = begin(packet);
    if (le::get8(p) != tag) throw Error {};
}

std::ostream& operator<< (std::ostream& os, const rx::Packet& val)
{
    const void* a = val.val.data();
    const void* const b = val.val.data() + val.val.size();

    while (a!=b) {
	char buf[16*3];
	a = hexdump(buf, sizeof buf, a, b);
	os << buf << '\n';
    }
    return os;
}
