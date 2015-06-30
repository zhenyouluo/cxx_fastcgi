SRC=$(wildcard *.cc)
DIR=$(notdir $(SRC))
OBJS=$(DIR:%.cc=%.o)
LIB=libbase.so

CXX=g++
CXXFLAGS=-g -fPIC
INCS=-I. \
	-I..
LIBS=
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



libbase.so : $(OBJS)
	$(CXX) -shared -fPIC -o $@ $^ $(LIBS)
%.o : %.cc
	$(CXX) -c -o $@ $^ $(CXXFLAGS) $(INCS)

