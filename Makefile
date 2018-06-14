# Begin Makefile
# Top-level Makefile for kvs

include Makefile.inc

exeobjs=dbutil
libobjs=

all: rmlog $(libobjs) $(exeobjs)
full: clean all

rmlog:
	@> $(LOG)

# SUB-TREE TARGETS
libx_all:
	@$(MAKE) -C libx
libx_clean:
	@$(MAKE) -C libx clean
libx_full:
	@$(MAKE) -C libx full
	
libkvs_all:
	@$(MAKE) -C libkvs
libkvs_clean:
	@$(MAKE) -C libkvs clean
libkvs_full:
	@$(MAKE) -C libkvs full

clean: libx_clean libkvs_clean
	-rm -f *.o *.la *.a *.lo *.so gmon.out $(exeobjs)
	@> $(LOG)

envoy_install:
	@$(MAKE) $(SUBMAKEFLAGS) -C $(ENVOY_SRCDIR) install
	
#install:
#	cp ....

dbutilobj=dbutil.o test_kvs_node.o test_kvs.o
dbutil: $(dbutilobj) libx_all libkvs_all
	@$(LD_NOTICE)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o dbutil $(dbutilobj) -lsqlite3 -lkvs -lx \
		2>&1 | tee -a $(LOG)

# End Makefile