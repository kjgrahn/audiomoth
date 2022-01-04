/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_UTF8_H
#define AUDIOMOTH_UTF8_H

#include <cwctype>
#include <string>

namespace utf8 {

    /**
     * Turn a wide C string (supposedly UCS-32 on Unix) into an
     * UTF-8-encoded string.
     */
    std::string encode(const wchar_t* s);
}

#endif
