# Begin Makefile.inc

SHELL=/bin/sh
LOG=log

# Not targeting any other platform as of Aug, 2008
TARGET=LINUX

# BUILD_MODE:
# DEBUG or RELEASE
BUILD_MODE=DEBUG

# Notices
CC_NOTICE=printf "%-24s |__ compiling --> $@\n" $*.c | tee -a $(LOG)
AR_NOTICE=printf "%-24s ( archiving )\n" $@ | tee -a $(LOG)
LD_NOTICE=printf "%-24s ( linking )\n" $@ | tee -a $(LOG)

# Build flags
CC=/usr/bin/cc
AR=/usr/bin/ar
ARFLAGS=r
PLATFORM_FLAGS=
INCLUDE_PATH=-I include
LIB_FLAGS=-L.

ifeq ($(BUILD_MODE), DEBUG)
#$(warning > Building targets in DEBUG mode.)
	BUILD_FLAGS=-g -DDEBUG
else
#$(warning > Building targets in RELEASE mode.)
	BUILD_FLAGS=-O2
endif

EXTRA_CFLAGS=$(PLATFORM_FLAGS) $(INCLUDE_PATH) $(LIB_FLAGS) $(BUILD_FLAGS)
CFLAGS=-D_$(TARGET) $(EXTRA_CFLAGS)

.c.o: 
	@$(CC_NOTICE)
	@$(CC) $(CFLAGS) -o $*.o -c $*.c 2>&1 | tee -a $(LOG)

# End Makefile.inc