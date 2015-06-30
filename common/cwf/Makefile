DEPTH=../../
SRC=$(wildcard *.cc)
DIR=$(notdir $(SRC))
OBJS=$(DIR:%.cc=%.o)
LIB=libcwf.so

CXX=g++
CXXFLAGS=-g -fPIC
INCS=-I. \
	-I$(DEPTH)/common/ \
	-I$(DEPTH)/third-64/fcgi/include/

LIBS=-L$(DEPTH)/common/base/output/so \
	-L$(DEPTH)/third-64/fcgi/lib \
	-lbase \
	-lfcgi \
	-lpthread
all : $(LIB) output

clean:
	rm -rf output
	rm -f $(LIB)
	rm -f $(OBJS)

output:
	mkdir -p output/so
	cp $(LIB) output/so
	mkdir -p output/include
	cp *.h output/include


libcwf.so : $(OBJS)
	$(CXX) -shared -fPIC -o $@ $^ $(LIBS)
%.o : %.cc
	$(CXX) -c -o $@ $^ $(CXXFLAGS) $(INCS)

