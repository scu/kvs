# Begin Makefile
# libx Makefile

include ../Makefile.inc

INCLUDE_PATH=-I ../include

exeobjs=
libobjs=libx

all: rmlog $(libobjs) $(exeobjs)
full: clean all

rmlog:
	@> $(LOG)

clean:
	-rm -f *.o *.la *.a *.lo *.so ../libx.a gmon.out $(exeobjs)
	@> $(LOG)

libxobj=messages.o system.o strings.o bstrlib.o
libx: $(libxobj)
	@$(AR_NOTICE)
	@$(AR) $(ARFLAGS) ../libx.a $(libxobj) \
		2>&1 | tee -a $(LOG)

# End Makefile
