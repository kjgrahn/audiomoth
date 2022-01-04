/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "utf8.h"

std::string utf8::encode(const wchar_t* ws)
{
    std::string s;
    auto octet = [&s] (uint8_t mask, uint8_t n) {
	s.push_back(mask | n);
    };
    auto hextet = [&s] (uint8_t n) {
	n &= 0x3f;
	s.push_back(0x80 | n);
    };
    
    const auto* p = ws;
    while (*p) {
	const wchar_t w = *p++;
	if (!w) break;

	if (w < 0x80) {
	    s.push_back(w);
	}
	else if (w < 0x00000800) {
	    octet(0xc0, w >> 6);
	    hextet(w);
	}
	else if (w < 0x00010000) {
	    octet(0xe0, w >> 12);
	    hextet(w >> 6);
	    hextet(w);
	}
	else if (w < 0x00200000) {
	    octet(0xf0, w >> 18);
	    hextet(w >> 12);
	    hextet(w >> 6);
	    hextet(w);
	}
	else if (w < 0x04000000) {
	    octet(0xf8, w >> 24);
	    hextet(w >> 18);
	    hextet(w >> 12);
	    hextet(w >> 6);
	    hextet(w);
	}
	else {
	    octet(0xfc, w >> 30);
	    hextet(w >> 24);
	    hextet(w >> 18);
	    hextet(w >> 12);
	    hextet(w >> 6);
	    hextet(w);
	}
    }
    
    return s;
}
