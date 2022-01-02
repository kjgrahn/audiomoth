#
# Makefile
#
# Copyright (c) 2010--2022 J�rgen Grahn
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local
CXXFLAGS=-Wall -Wextra -pedantic -std=c++14 -g -Os -Wold-style-cast
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

libaudiomoth.a: foo.o
libaudiomoth.a: foo.o
libaudiomoth.a: foo.o
	$(AR) $(ARFLAGS) $@ $^

audiomoth: audiomoth.o libaudiomoth.a
	$(CXX) $(CXXFLAGS) -o $@ audiomoth.o -L. -laudiomoth

libtest.a: test/foo.o
libtest.a: test/foo.o
libtest.a: test/foo.o
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
	$(RM) {,test/}*.o
	$(RM) lib*.a
	$(RM) test.cc tests
	$(RM) TAGS
	$(RM) -r dep/

love:
	@echo "not war?"

$(shell mkdir -p dep/{,test})
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/test/%.d: ;
-include dep/*.d
-include dep/test/*.d
