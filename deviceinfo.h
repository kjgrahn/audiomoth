/* -*- c++ -*-
 *
 * Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#ifndef AUDIOMOTH_DEVICEINFO_H
#define AUDIOMOTH_DEVICEINFO_H

#include <string>
#include <vector>

struct hid_device_info;

namespace hid {

    /**
     * libhidapi struct hid_device_info as a C++ class.
     */
    struct DeviceInfo {
	std::string path;
	unsigned short vendor_id;
	unsigned short product_id;
	std::wstring serial_number;
	unsigned short release_number;
	std::wstring manufacturer_string;
	std::wstring product_string;
	unsigned short usage_page;
	unsigned short usage;
	int interface_number;

	explicit DeviceInfo(const hid_device_info& di);
    };

    std::vector<DeviceInfo> enumerate(unsigned short vendor_id,
				      unsigned short product_id);
}

#endif
