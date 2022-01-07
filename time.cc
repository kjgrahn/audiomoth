/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "put_time.h"

#include <iostream>

namespace {

    std::ostream& put(std::ostream& os,
		      struct tm* (*f) (const time_t *),
		      const time_t t)
    {
	const tm tm = *f(&t);
	char buf[30];
	std::strftime(buf, sizeof buf, "%F %H:%M:%S %z", &tm);
	return os << buf;
    }
}

std::ostream& put_time(std::ostream& os, const time_t t)
{
    return put(os, std::localtime, t);
}

std::ostream& put_utc(std::ostream& os, const time_t t)
{
    return put(os, std::gmtime, t);
}
