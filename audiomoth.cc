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

namespace {

    const char* version()
    {
	return "1.0";
    }

}

int main(int argc, char ** argv)
{
    const std::string prog = argv[0] ? argv[0] : "audiomoth";
    const std::string usage = std::string("usage: ")
	+ prog + " [-u user] [-o file] [file]\n"
	"       "
	+ prog + " --help\n"
	"       "
	+ prog + " --version";
    const char optstring[] = "u:o:";
    const struct option long_options[] = {
	{"help", 0, 0, 'H'},
	{"version", 0, 0, 'V'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    std::string user;
    std::string outfile;

    int ch;
    while ((ch = getopt_long(argc, argv,
			     optstring,
			     &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'u':
	    user = optarg;
	    break;
	case 'o':
	    outfile = optarg;
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

    const std::vector<char*> files(argv+optind, argv+argc);
    if (files.size() > 1) {
	std::cerr << "error: too many arguments\n"
		  << usage << '\n';
	return 1;
    }

    auto& os = std::cout;
    for (const auto& di : hid::enumerate(0x10c4, 0x0002)) {
	os << di.path << '\n';
    }

    return 0;
}
