#MAKEFILE FOR libPCSC4Java32bits.so

OS := $(shell uname -m)
DEBUG = NONE

COMPILE = $(CC) $(INCLUDE) $(CFLAGS)
INCLUDE=-I. -I./include -I/usr/include/PCSC/

CC = gcc

ifneq	($(OS), x86_64)
	ifeq	($(DEBUG), NONE)
		CFLAGS = -O2 -Wall -W -fPIC -shared -D_REENTRANT -D LINUX32 
		FINAL_DIR = release/32_bits
	else
CFLAGS = -O2 -Wall -W -fPIC -shared -D_REENTRANT -D LINUX32 -D _DEBUG 
		FINAL_DIR = debug/32_bits
	endif

	FINAL_LIB = libPCSC4Java32bits.so
	
else
	ifeq	($(DEBUG), NONE)
		CFLAGS = -O2 -Wall -W -fPIC -shared -D_REENTRANT -D LINUX64 
		FINAL_DIR = release/64_bits
	else
		CFLAGS = -O2 -Wall -W -fPIC -shared -D_REENTRANT -D LINUX64 -D _DEBUG 
		FINAL_DIR = debug/64_bits
	endif

	FINAL_LIB = libPCSC4Java64bits.so

endif

LDFLAGS=-lstdc++ -lpcsclite

lib := PCSC

libJNI-obj := \
	     $(lib).o

all: $(FINAL_LIB)

$(FINAL_LIB) :  $(libJNI-obj)  
		$(COMPILE) $(libJNI-obj) -o $(FINAL_LIB) $(LDFLAGS)
		
$(lib).o : ../src/$(lib).cpp
		$(COMPILE) -c -o $(lib).o ../src/$(lib).cpp

install: 
	cp -f $(FINAL_LIB) ../binaries/$(FINAL_DIR)/$(FINAL_LIB)
	rm -f $(FINAL_LIB)

clean:
	rm -f *.o
	rm -f *~
	rm -f ../binaries/$(FINAL_DIR)/$(FINAL_LIB)

