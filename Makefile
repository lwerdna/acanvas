FLAGS_FLTK = $(shell fltk-config --use-images --cxxflags)
LD_FLTK = $(shell fltk-config --use-images --ldstaticflags)
LD_FLAGS = -L/usr/local/lib
LINK = $(CXX)

#--<BOILERPLATE>---------------------------------------------------------------
CFLAGS = -I/usr/local/include
CPPFLAGS = -std=c++11 -I/usr/local/include
# OS DETECTION -> set compiler, define variable for source files to know
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    $(info ******** compiling for MACOS! ********)
    CC = clang
    CPP = clang++
    CFLAGS += -D OS_IS_MACOS
    CPPFLAGS += -D OS_IS_MACOS
endif
ifeq ($(UNAME_S),Linux)
    $(info ******** compiling for LINUX! ********)
    CC = gcc
    CPP = g++
    CFLAGS += -D OS_IS_LINUX
    CPPFLAGS += -D OS_IS_LINUX
endif
ifeq ($(UNAME_S),FreeBSD)
    $(info ******** compiling for FreeBSD! ********)
    CC = cc
    CFLAGS += -D OS_IS_FREEBSD
    CPPFLAGS += -D OS_IS_FREEBSD
endif
ifeq (CC,"")
    $(error unable to detect OS!)
endif

# release will be empty unless supplied on make command line
# eg: $ RELEASE=1 make
RELEASE ?=
ifeq ($(RELEASE),)
$(info MAKEFILE DEBUG MODE, PRINTS PRESENT)
CFLAGS += -g -O0 -DDEBUG=1
CPPFLAGS += -g -O0 -DDEBUG=1
else
$(info MAKEFILE RELEASE MODE)
endif
#--</BOILERPLATE>--------------------------------------------------------------

OBJS = ACanvas.o Driver.o logic.o

all: $(OBJS) acanvas 

%.o: %.cxx %.h
	$(CPP) $(CPPFLAGS) $(FLAGS_FLTK) -c $<


acanvas: $(OBJS)
	$(LINK) $(LD_FLTK) $(OBJS) -o acanvas $(LD_FLAGS) $(LD_FLTK) -lautils -lafltk

install:
	install ./acanvas /usr/local/bin

clean: 
	rm -f $(OBJS) 2> /dev/null
	rm acanvas 2> /dev/null
