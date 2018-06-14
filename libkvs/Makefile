# Begin Makefile
# libkvs Makefile

include ../Makefile.inc

INCLUDE_PATH=-I ../include

exeobjs=
libobjs=libkvs

all: rmlog $(libobjs) $(exeobjs)
full: clean all

rmlog:
	@> $(LOG)

clean:
	-rm -f *.o *.la *.a *.lo *.so ../libkvs.a gmon.out $(exeobjs)
	@> $(LOG)
	
libkvsobj=kvs.o node.o
libkvs: $(libkvsobj)
	@$(AR_NOTICE)
	@$(AR) $(ARFLAGS) ../libkvs.a $(libkvsobj) \
		2>&1 | tee -a $(LOG)

# End Makefile
