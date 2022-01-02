/* Copyright (c) 2022 Jörgen Grahn
 * All rights reserved.
 */
#include "deviceinfo.h"

#include <hidapi/hidapi.h>

using hid::DeviceInfo;

namespace {
    template <class Ch>
    std::basic_string<Ch> get(const Ch* p)
    {
	if (!p) return {};
	return {p};
    }
}

DeviceInfo::DeviceInfo(const hid_device_info& di)
    : path {get(di.path)},
      vendor_id {di.vendor_id},
      product_id {di.product_id},
      serial_number {get(di.serial_number)},
      release_number {di.release_number},
      manufacturer_string {get(di.manufacturer_string)},
      product_string {get(di.product_string)},
      usage_page {di.usage_page},
      usage {di.usage},
      interface_number {di.interface_number}
{}

std::vector<DeviceInfo> hid::enumerate(unsigned short vendor_id,
				       unsigned short product_id)
{
    hid_device_info* const p0 = hid_enumerate(vendor_id, product_id);

    std::vector<DeviceInfo> v;
    const hid_device_info* p = p0;
    while (p) {
	v.emplace_back(*p);
	p = p->next;
    }

    hid_free_enumeration(p0);
    return v;
}
