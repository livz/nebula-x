FAKELIB=/var/tmp/flag15/tls/i686/sse2/cmov/libc.so.6
FAKEOBJ=libc.o
FAKESRC=libc.c
VER_SCRIPT=version.script

all: $(FAKESRC)
	gcc -Wall -fPIC -o $(FAKEOBJ) -c $(FAKESRC)
	gcc -shared -Wl,-Bstatic,--version-script=$(VER_SCRIPT),-soname,libc.so.6 -o $(FAKELIB) $(FAKEOBJ) -static

clean:
	rm $FAKEOBJ

cleanall:
	rm *.o *.c $(VER_SCRIPT) 