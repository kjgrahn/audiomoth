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
#include <sstream>
#include <vector>
#include <string>

#include <getopt.h>
#include <hidapi/hidapi.h>

#include "deviceinfo.h"
#include "device.h"
#include "message.h"
#include "cfg/config.h"

namespace {

    const char* version()
    {
	return "1.0";
    }

    std::ostream& complain(std::ostream& os, const option* p,
			   int ch, const char* arg)
    {
	while(p->val && p->val != ch) p++;
	if (!p->val) return os;
	if (!p->has_arg) {
	    return os << "error: bad option " << p->name;
	}
	return os << "error: bad --" << p->name << " argument \"" << arg << '"';
    }

    bool wrong_uid(hid::Device& dev, std::ostream& os, const std::string& ref)
    {
	if (ref.empty()) return false;

	std::ostringstream oss;
	oss << write(dev, tx::GetUid{});
	const auto uid = oss.str();
	if (uid==ref) return false;

	os << "error: uid mismatch: " << uid << '\n';
	return true;
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
	+ prog + " [--uid uid] [-i]\n"
	"       "
	+ prog + " [--uid uid] -T\n"
	"       "
	+ prog + " [--uid uid] [config options ...]\n"
	"       "
	+ prog + " --help\n"
	"       "
	+ prog + " --version";
    const char optstring[] = "iTt:";
    const struct option long_options[] = {
	{"uid",    1,0, 'u'},
	{"time",   1,0, 't'},
	{"cyclic", 1,0, 'C'},        {"highpass", 1,0,'p'},
	{"dates",  1,0, 'D'},        {"lowpass",  1,0,'q'},
	{"gain",   1,0, 'G'},        {"bandpass", 1,0,'r'},
	{"rate",   1,0, 'R'},
	{"elephants",    0,0, 'E'},  {"NiMH",     0,0,'N'},
	{"need-chime",   0,0, '*'},  {"LiPo",     0,0,'N'},
	{"energy-saver", 0,0, 'e'},  {"alkaline", 0,0,'a'},
	{"ignore-low-battery", 0,0, 'b'},
	{"no-rw-led",          0,0, 'L'},
	{"no-battery-led",     0,0, 'B'},
	{"help",    0, 0, 'H'},
	{"version", 0, 0, 'V'},
	{0, 0, 0, 0}
    };

    struct {
	bool set_time = false;
	bool configure = false;
	std::string uid;
    } opt;
    Config c;

    int ch;
    while ((ch = getopt_long(argc, argv,
			     optstring,
			     &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'i': opt.set_time  = false;
		  opt.configure = false;
		  break;
	case 'T': opt.set_time  = true;
		  opt.configure = false;
		  break;
	case 'u': opt.uid = optarg; break;

	case 't': if (!c.periods.add(optarg)) {
		      std::cerr << "error: bad time period " << optarg << '\n'
				<< usage << '\n';
		      return 1;
		  }
	          break;

	case 'C': c.cyclic = cfg::Cyclic {optarg}; break;
	case 'D': c.dates = cfg::Dates {optarg}; break;
	case 'G': c.gain = cfg::Gain {optarg}; break;
	case 'R': c.samplerate = cfg::SampleRate {optarg}; break;
	case 'p': c.filter = {cfg::PassFilter::HIGH, optarg}; break;
	case 'q': c.filter = {cfg::PassFilter::LOW, optarg}; break;
	case 'r': c.filter = {cfg::PassFilter::BAND, optarg}; break;

	case 'E': c.elephants = true; break;
	case '*': c.need_chime = true; break;
	case 'e': c.save_energy = true; break;
	case 'N': c.nimh_batteries = true; break;
	case 'a': c.nimh_batteries = false; break;
	case 'b': c.ignore_low_battery = true; break;
	case 'L': c.rw_led = false; break;
	case 'B': c.battery_led = false; break;

	case 'H':
	    std::cout << usage << '\n';
	    return 0;
	    break;
	case 'V':
	    std::cout << "audiomoth " << version() << '\n'
		      << "Copyright (c) 2022 Jörgen Grahn. "
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

	switch(ch) {
	case 'i':
	case 'T':
	case 'u':
	    break;
	default:
	    opt.set_time  = false;
	    opt.configure = true;
	    break;
	}

	if (!c.valid()) {
	    /* Ignoring the possibility that further options may make
	     * it valid again: there's no good reason to feed bad
	     * options ... and here's a convincing reason not to.
	     */
	    complain(std::cerr,
		     long_options,
		     ch, optarg) << '\n'
				 << usage << '\n';
	    return 1;
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

	if (wrong_uid(dev, std::cerr, opt.uid)) {
	    return 1;
	}

	if (opt.set_time) {
	    return set_time(dev, os, std::time(nullptr));
	}
	else if (opt.configure) {
	    // ...
	}
	else {
	    return info(dev, os);
	}
    }
    catch (const hid::Error&) {
	std::cerr << "error: " << dev.error << '\n';
	return 1;
    }

    return 0;
}
