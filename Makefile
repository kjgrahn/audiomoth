#
# Makefile
#
# Copyright (c) 2010--2022 Jörgen Grahn
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local
CXXFLAGS=-Wall -Wextra -pedantic -std=c++14 -g -Os -Wold-style-cast
CFLAGS=-W -Wall -pedantic -std=c99 -g -Os
CPPFLAGS=
ARFLAGS=rTP

.PHONY: all
all: audiomoth
all: tests

.PHONY: install
install: audiomoth
install: audiomoth.1
	install -m755 audiomoth $(INSTALLBASE)/bin/
	install -m644 audiomoth.1 $(INSTALLBASE)/man/man1/

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

libaudiomoth.a: deviceinfo.o
libaudiomoth.a: device.o
libaudiomoth.a: time.o
libaudiomoth.a: message.o
libaudiomoth.a: rx.o
libaudiomoth.a: utf8.o
libaudiomoth.a: hexdump.o
libaudiomoth.a: split.o
libaudiomoth.a: ranges.o
libaudiomoth.a: cfg/timeperiods.o
libaudiomoth.a: cfg/cyclic.o
libaudiomoth.a: cfg/passfilter.o
libaudiomoth.a: cfg/gain.o
libaudiomoth.a: cfg/samplerate.o
	$(AR) $(ARFLAGS) $@ $^

audiomoth: audiomoth.o libaudiomoth.a
	$(CXX) $(CXXFLAGS) -o $@ audiomoth.o -L. -laudiomoth -lhidapi-hidraw

libtest.a: test/utf8.o
libtest.a: test/hexdump.o
libtest.a: test/split.o
libtest.a: test/ranges.o
libtest.a: test/endian.o
libtest.a: test/time.o
libtest.a: test/timeperiods.o
libtest.a: test/cyclic.o
libtest.a: test/passfilter.o
libtest.a: test/gain.o
libtest.a: test/samplerate.o
	$(AR) $(ARFLAGS) $@ $^

test/%.o: CPPFLAGS+=-I.

test.cc: libtest.a
	orchis -o$@ $^

tests: test.o libaudiomoth.a libtest.a
	$(CXX) $(CXXFLAGS) -o $@ test.o -L. -ltest -laudiomoth

.PHONY: tags TAGS
tags: TAGS
TAGS:
	ctags --exclude='test' -eR . --extra=q

.PHONY: clean
clean:
	$(RM) audiomoth
	$(RM) {,test/,cfg/}*.o
	$(RM) lib*.a
	$(RM) test.cc tests
	$(RM) TAGS
	$(RM) -r dep/

love:
	@echo "not war?"

$(shell mkdir -p dep/{,cfg,test})
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/cfg/%.d: ;
dep/test/%.d: ;
-include dep/*.d
-include dep/cfg/*.d
-include dep/test/*.d
