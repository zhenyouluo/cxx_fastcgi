DEPTH=../../

PLUMA_SRCS=$(wildcard Pluma/src/Pluma/*.cpp)
PLUMA_OBJS=$(patsubst %.cpp, %.o, $(PLUMA_SRCS))

SAMRT_SRCS=$(wildcard interface/*.cpp)
SAMRT_OBJS=$(patsubst %.cpp, %.o, $(SAMRT_SRCS))

CXXFLAGS= -g -fPIC

INCS= -I. \
	  -I./Pluma/include \
	  -I./Pluma/src \
	  -I./cwf \
	  -I/usr/include/boost \
	  -I$(DEPTH)/common \
	  -I$(DEPTH)/common/base/output/include \
	  -I$(DEPTH)/common/cwf/output/include \
	  -I$(DEPTH)/third-64/fcgi/include \
	  $(NULL)

BIN_INCS= -I./interface \
		  -I./host \
		  -I$(DEPTH)/common \
		  -I$(DEPTH)/common/base/output/include \
		  -I$(DEPTH)/common/cwf/output/include \
		  -I$(DEPTH)/third-64/fcgi/include \
		  -I$(DEPTH)/third-64/clog/include \
		  -I$(DEPTH)/third-64/bsl/include \
		  $(NULL)

BIN_LIBS= -L$(DEPTH)/third-64/clog/lib \
		  -L$(DEPTH)/third-64/bsl/lib \
		  -L$(DEPTH)/third-64/fcgi/lib \
		  -L$(DEPTH)/common/cwf/output/so \
		  -L$(DEPTH)/common/base/output/so \
		  -llog \
		  -lfcgi \
		  -lcwf \
		  -lbase \
		  $(NULL)

BIN_SRCS=$(wildcard host/*.cpp)
BIN_OBJS=$(patsubst %.cpp, %.o, $(BIN_SRCS))

LIB=libsmart_plugin_frame.so

BIN=host/service_main

all : $(LIB) $(BIN) output

libsmart_plugin_frame.so : $(PLUMA_OBJS) $(SAMRT_OBJS)
	$(CXX) -fPIC -shared -g -o $@ $^

bin : $(BIN)

clean :
	rm -rf output
	rm -rf $(PLUMA_OBJS) $(SAMRT_OBJS) host/*.o $(BIN)  $(LIB)

output : $(LIB) $(BIN)
	mkdir -p output/include/Pluma
	mkdir -p output/bin
	mkdir -p output/conf
	mkdir -p output/so
	cp $(LIB) output/so
	cp $(BIN) output/bin
	cp interface/*.h output/include
	cp Pluma/include/Pluma/*.* output/include/Pluma
	cp conf/*.conf output/conf

%.o : %.cpp
	$(CXX) -c -o $@ $^ $(CXXFLAGS) $(INCS) $(BIN_INCS)

host/service_main : host/service_main.o $(LIB)
	$(CXX) -rdynamic -o host/service_main $(filter %.o, $^) $(CXXFLAGS) $(BIN_INCS) -L . -lsmart_plugin_frame $(BIN_LIBS) -ldl -lpthread

tools/static_topo : tools/StaticTopo.o $(LIB)
	$(CXX) -rdynamic -o $@ $(filter %.o, $^) $(CXXFLAGS) $(BIN_INCS) -L . -lsmart_plugin_frame $(BIN_LIBS) -ldl -lpthread

$(BIN_OBJS) : %.o : %.cpp
	$(CXX) -c -o $@ $^ $(CXXFLAGS) $(BIN_INCS) $(INCS)
