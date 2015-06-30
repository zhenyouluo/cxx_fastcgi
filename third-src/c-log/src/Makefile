SRCS=$(wildcard *.c)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
CXXFLAGS= -g -fPIC
CXX=g++

LIB=clog.a


all : $(LIB) output

clog.a : $(SRCS)
	$(CXX) -c -fPIC log.c
	ar crv liblog.a log.o


clean :
	rm -rf output
	rm *.o

output :
	mkdir output/lib -p
	mkdir output/include -p
	cp *.a output/lib
	cp *.h output/include
