##########################################
# Options
##########################################
GPUTK_LIB_PATH=$(CURDIR)/lib
GPUTK_SRC_PATH=$(CURDIR)
CXX=g++

##########################################
##########################################

DEFINES=-DGPUTK_USE_COURSERA -DGPUTK_USE_JSON11=0
CXX_FLAGS=-fpic -O3 -g -std=c++11 -I . -I $(GPUTK_SRC_PATH) # -I /usr/local/cuda/include -L /usr/local/cuda/lib64 
LIBS=-lm -std=c++11 -L $(GPUTK_LIB_PATH) # -lcuda

##########################################
##########################################

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIBS += -lrt
endif

##########################################
##########################################

SOURCES := $(shell find $(GPUTK_SRC_PATH) ! -name "*_test.cpp" -name "*.cpp")
TESTS :=  $(shell find $(GPUTK_SRC_PATH) -name "*_test.cpp")

OBJECTS = $(SOURCES:.cpp=.o)

##############################################
# OUTPUT
##############################################

.PHONY: all
.SUFFIXES: .o .cpp
all: libgputk.so

.cpp.o:
	$(CXX) $(DEFINES) $(CXX_FLAGS) -c -o $@ $<

libgputk.so: $(OBJECTS)
	mkdir -p $(GPUTK_LIB_PATH)
	$(CXX) -shared -o $(GPUTK_LIB_PATH)/$@ $(OBJECTS) $(LIBS)

libgputk.a: $(OBJECTS)
	mkdir -p $(GPUTK_LIB_PATH)
	ar rcs -o $(GPUTK_LIB_PATH)/$@ $(OBJECTS)

test: libgputk.so
	$(CXX) $(DEFINES) $(CXX_FLAGS) -o $@ $(TESTS) -lgputk $(LIBS)


clean:
	rm -fr lib test
	-rm -f $(EXES) *.o *~
