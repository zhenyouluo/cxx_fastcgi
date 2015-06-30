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
LIB=libactions.so
SRCS=$(wildcard *.cpp)
DIR=$(notdir $(SRCS))
OBJS=$(DIR:%.cpp=%.o)

DEPTH=../../
INCS=-I. \
   -I.. \
   -I$(DEPTH)/common \
   -I$(DEPTH)/third-64/fcgi/include \
   -I$(DEPTH)/third-64/bsl/include 

WORKROOT ?= ../../../
LIBS  = -L./ \
		-L$(DEPTH)/common/cwf/output/so/ \
		-L$(DEPTH)/common/base/output/so/ \
	    	-L$(DEPTH)/third-64/fcgi/lib \
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

output:
	mkdir -p output/so
	cp $(LIB) output/so


%.o : %.cpp
	$(CXX) -c -g -o $@ $^ $(CXXFLAGS) $(INCS)
#guideaction.o : ../../actions/guideaction.cpp
#	$(CXX) -c -g -o $@ $^ $(CXXFLAGS) $(INCS)
libactions.so : $(OBJS)
	@echo $(SRCS)
	$(CXX)  -o $@ $^ $(LIBS)
	#$(CXX) -shared -fPIC -o $@ $^ $(LIBS)
endif #ifeq ($(shell uname -m),x86_64)


