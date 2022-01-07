/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_TIME_H
#define AUDIOMOTH_TIME_H

#include <iosfwd>
#include <ctime>

std::ostream& put_time(std::ostream& os, const time_t t);
std::ostream& put_utc(std::ostream& os, const time_t t);

#endif
