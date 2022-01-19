/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "message.h"
#include "endians.h"

hid::Packet tx::GetTime::encode() const
{
    hid::Packet p {};
    auto it = begin(p);
    be::put16(it, tag);
    return p;
}

hid::Packet tx::SetTime::encode() const
{
    hid::Packet p {};
    auto it = begin(p);
    be::put16(it, tag);
    le::put32(it, t);
    return p;
}

hid::Packet tx::GetUid::encode() const
{
    hid::Packet p {};
    auto it = begin(p);
    be::put16(it, tag);
    return p;
}

hid::Packet tx::GetBattery::encode() const
{
    hid::Packet p {};
    auto it = begin(p);
    be::put16(it, tag);
    return p;
}

hid::Packet tx::SetAppPacket::encode() const
{
    hid::Packet p {};
    auto it = begin(p);
    be::put16(it, tag);
    le::put32(it, t);
    config.encode(it);
    return p;
}

hid::Packet tx::GetFirmwareVersion::encode() const
{
    hid::Packet p {};
    auto it = begin(p);
    be::put16(it, tag);
    return p;
}

hid::Packet tx::GetFirmwareDescription::encode() const
{
    hid::Packet p {};
    auto it = begin(p);
    be::put16(it, tag);
    return p;
}
