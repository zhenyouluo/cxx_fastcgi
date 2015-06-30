#COMAKE2 edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
CC=gcc
CXX=g++
CXXFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CPPFLAGS=-D_GNU_SOURCE \
  -g \
  -D__STDC_LIMIT_MACROS \
  -DVERSION=\"1.9.8.7\"
LIB=libprepare.so
SRCS=$(wildcard *.cpp)
DIR=$(notdir $(SRCS))
OBJS=$(DIR:%.cpp=%.o)

ACTION_SRCS=$(wildcard ../../web-requests/*.cpp)
ACTION_OBJS=$(patsubst %.cpp, %.o, $(ACTION_SRCS)) 
DEPTH=../../../
INCS=-I. \
   -I.. \
   -I$(DEPTH)/service/web-frame/output/include/ \
   -I$(DEPTH)/common/cwf/output/include \
   -I$(DEPTH)/common/base/output/include \
   -I$(DEPTH)/common \
   -I$(DEPTH)/service/web-requests \
   -I$(DEPTH)/third-64/fcgi/include \
   -I$(DEPTH)/third-64/clog/include

WORKROOT ?= ../../../
LIBS  = -L./ \
		-L$(DEPTH)/service/web-frame/output/so/ \
		-L$(DEPTH)/common/cwf/output/so/ \
		-L$(DEPTH)/common/base/output/so/ \
	    	-L$(DEPTH)/third-64/fcgi/lib \
	    	-L$(DEPTH)/third-64/clog/lib \
		-llog \
		-lsmart_plugin_frame \
		-lcwf \
		-lbase \
		-lfcgi \
		-ldl \
		-lpthread 
	#============ CCP vars ============
CCP_FLAGS=


#COMAKE UUID
all : $(LIB) output

clean:
	rm -rf output
	rm -rf $(LIB)
	rm -rf $(OBJS)
	rm -rf $(ACTION_OBJS)

output:
	mkdir -p output/so
	cp $(LIB) output/so


%.o : %.cpp
	$(CXX) -c -g -o $@ $^ $(CXXFLAGS) $(INCS)
#	$(CXX) -c -g -o $@ $^ $(CXXFLAGS) $(INCS)
libprepare.so : $(OBJS) $(ACTION_OBJS)
	$(CXX) -shared -fPIC -o $@ $^ $(LIBS)
endif #ifeq ($(shell uname -m),x86_64)


