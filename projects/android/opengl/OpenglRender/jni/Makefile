#SYSROOT=/opt/android_build/sysroot
SYSROOT=/opt/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi

PROJECT_ROOT=../

#cc=/opt/android_build/bin/arm-linux-androideabi-gcc  --sysroot=$(SYSROOT)
cc=/opt/arm-unknown-linux-gnueabi-4.5.3-glibc/arm-unknown-linux-gnueabi/bin/gcc

CFLAGS = -g -O2 -fPIC -DANDROID  -DHAVE_PTHREADS -w -DENABLE_LOG

CFLAGS +=	\
		-I$(SYSROOT)/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include \
		-I$(SYSROOT)/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include-fixed \
		-I$(SYSROOT)/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include/c++ \
		-I$(SYSROOT)/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include/c++/arm-unknown-linux-gnueabi \
		-I$(SYSROOT)/usr/include	\
		-I$(SYSROOT)/usr/include/linux \
		-IOEM/include/media/libstagefright \
		-IOEM/include/include/media \
		-IOEM/include/include \
		-IOEM/include/android	\
		-IOEM/include/include/utils \
		-IOEM/include/include/media/stagefright \
		-IOEM/include/include/media/stagefright/openmax \
		-IOEM/include/frameworks/base/core/jni \
		-IOEM/include/hardware/libhardware/include \
		-IOEM/include/system/core/include \
		-IOEM/include/skia/include \
		-IOEM/include/pvmi/pvmf/include \
		-IOEM/include/extern_libs_v2/khronos/openmax/include \
		-Iinclude	\
		-I$(PROJECT_ROOT)src/utils	\
		-I$(PROJECT_ROOT)include	

SDFLAGS = -nostdlib -Wl,-shared -L$(SYSROOT)/usr/lib  -lgcc -lc -lm  -ldl -L. -ltest-ubmedia
#SDFLAGS = -nostdlib -Wl,-T,armelf.xsc -Wl,-Bsymbolic -Wl,-soname,$@ -lc


OBJECT=	test-jni.o

all : libtest.so	

libtest.so : $(OBJECT)
	$(cc) $(SDFLAGS) -o  $@ $^ 
test-jni.o : test-jni.cpp
	$(cc) $(CFLAGS)  -nostdinc -c test-jni.cpp


clean:
	rm -rf *.o libtest.so
info:
	@echo $(CFLAGS)
