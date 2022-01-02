/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "message.h"
#include "endians.h"
#include "put_time.h"

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
    std::copy(p, p+8, begin(val));
}

std::ostream& operator<< (std::ostream& os, const rx::Uid& val)
{
    const char* prefix="";
    for (uint8_t c : val.val) {
	char buf[4];
	std::snprintf(buf, sizeof buf, "%s%02hhx", prefix, c);
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
