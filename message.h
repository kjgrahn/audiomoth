/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_MESSAGE_H
#define AUDIOMOTH_MESSAGE_H

#include "packet.h"

#include <ctime>
#include <string>
#include <iosfwd>

namespace rx {

    struct Error {};

    struct Time {
	Time(unsigned tag, const hid::Packet& p);
	time_t val;
    };

    struct Uid {
	Uid(unsigned tag, const hid::Packet& p);
	std::array<uint8_t, 8> val;
    };

    struct Battery {
	Battery(unsigned tag, const hid::Packet& p);
	uint8_t val;
    };

    struct Version {
	Version(unsigned tag, const hid::Packet& p);
	std::array<unsigned short, 3> val;
    };

    struct Description {
	Description(unsigned tag, const hid::Packet& p);
	std::string val;
    };
}

std::ostream& operator<< (std::ostream& os, const rx::Time& val);
std::ostream& operator<< (std::ostream& os, const rx::Uid& val);
std::ostream& operator<< (std::ostream& os, const rx::Battery& val);
std::ostream& operator<< (std::ostream& os, const rx::Version& val);
std::ostream& operator<< (std::ostream& os, const rx::Description& val);

namespace tx {

    template <unsigned Tag>
    struct Message {
	static constexpr unsigned tag = Tag;
    };

    class GetTime : public Message<1> {
    public:
	using response_t = rx::Time;
	hid::Packet encode() const;
    };

    class SetTime : public Message<2> {
    public:
	explicit SetTime(time_t t) : t{t} {}
	using response_t = rx::Time;
	hid::Packet encode() const;
    private:
	const time_t t;
    };

    class GetUid : public Message<3> {
    public:
	using response_t = rx::Uid;
	hid::Packet encode() const;
    };

    class GetBattery : public Message<4> {
    public:
	using response_t = rx::Battery;
	hid::Packet encode() const;
    };

    class GetAppPacket;
    class SetAppPacket;

    class GetFirmwareVersion : public Message<7> {
    public:
	using response_t = rx::Version;
	hid::Packet encode() const;
    };

    class GetFirmwareDescription : public Message<8> {
    public:
	using response_t = rx::Description;
	hid::Packet encode() const;
    };

    class QueryBootloader;
    class SwitchToBootloader;
}

#endif
