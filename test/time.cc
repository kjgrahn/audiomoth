/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <put_time.h>

#include <orchis.h>
#include <sstream>

#include <unistd.h>

namespace put {

    void time(orchis::TC)
    {
	std::ostringstream t1;
	put_time(t1, std::time(nullptr));
	sleep(1);
	std::ostringstream t2;
	put_time(t2, std::time(nullptr));

	orchis::assert_gt(t1.str().size(), 0);
	orchis::assert_neq(t1.str(), t2.str());
    }
}
