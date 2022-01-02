/*
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 or version 3 of the
 *    License.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * (Actually, you received no such copy. Sorry.)
 *
 */
#include <iostream>
#include <vector>
#include <string>

#include <getopt.h>
#include <hidapi/hidapi.h>

#include "deviceinfo.h"
#include "device.h"
#include "message.h"

namespace {

    const char* version()
    {
	return "1.0";
    }

    int info(hid::Device& dev, std::ostream& os)
    {
	os << "time:     " << write(dev, tx::GetTime{}) << '\n'
	   << "battery:  " << write(dev, tx::GetBattery{}) << '\n'
	   << "uid:      " << write(dev, tx::GetUid{}) << '\n'
	   << "firmware: " << write(dev, tx::GetFirmwareDescription{}) << '\n'
	   << "version:  " << write(dev, tx::GetFirmwareVersion{}) << '\n';
	return 0;
    }

    int set_time(hid::Device& dev, std::ostream& os,
		 const std::time_t t)
    {
	const auto resp = write(dev, tx::SetTime{t});
	os << resp << '\n';
	return 0;
    }
}

int main(int argc, char ** argv)
{
    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    const std::string prog = argv[0] ? argv[0] : "audiomoth";
    const std::string usage = std::string("usage: ")
	+ prog + " [-i]\n"
	"       "
	+ prog + " -T\n"
	"       "
	+ prog + " --help\n"
	"       "
	+ prog + " --version";
    const char optstring[] = "iT";
    const struct option long_options[] = {
	{"help", 0, 0, 'H'},
	{"version", 0, 0, 'V'},
	{0, 0, 0, 0}
    };

    struct {
	bool set_time = false;
    } opt;

    int ch;
    while ((ch = getopt_long(argc, argv,
			     optstring,
			     &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'i':
	    opt.set_time = false;
	    break;
	case 'T':
	    opt.set_time = true;
	    break;
	case 'H':
	    std::cout << usage << '\n';
	    return 0;
	    break;
	case 'V':
	    std::cout << "audiomoth " << version() << '\n'
		      << "Uses libhidapi " << hid_version_str() << '\n'
		      << "Copyright (c) 2022 J. Grahn. "
		      << "All rights reserved.\n";
	    return 0;
	    break;
	case ':':
	case '?':
	default:
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	}
    }

    if (optind != argc) {
	std::cerr << "error: too many arguments\n"
		  << usage << '\n';
	return 1;
    }

    hid::Device dev {0x10c4, 0x0002};
    if (!dev) {
	std::cerr << "error: failed to connect to AudioMoth: " << dev.error << '\n';
	return 1;
    }

    try {
	auto& os = std::cout;

	if (opt.set_time) {
	    return set_time(dev, os, std::time(nullptr));
	}

	return info(dev, os);
    }
    catch (const hid::Error&) {
	std::cerr << "error: " << dev.error << '\n';
	return 1;
    }

    return 0;
}
