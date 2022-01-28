/* Copyright (C) 2022 Jörgen Grahn.
 * All rights reserved.
 */
#include <message.h>

#include <orchis.h>

namespace rx {

    using orchis::TC;
    using orchis::assert_eq;

    namespace uid {

	void simple(TC)
	{
	    Uid val {3, {3, 0xfe, 0xb5, 0x37, 0x60, 0x02, 0x9b, 0x8d, 0x24}};
	    std::ostringstream oss;
	    oss << val;

	    assert_eq(oss.str(), "248d:9b02:6037:b5fe");
	}
    }
}
