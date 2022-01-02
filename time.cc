/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "put_time.h"

#include <iostream>

std::ostream& put_time(std::ostream& os, const time_t t)
{
    const tm tm = *std::gmtime(&t);
    char buf[20];
    std::strftime(buf, sizeof buf, "%F %H:%M:%S", &tm);
    return os << buf;
}
