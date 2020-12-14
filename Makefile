.ONESHELL:
.SECONDEXPANSION:
.PHONY: all clean

# Flags for the C++ compiler
CXXFLAGS = -std=c++20 -MD $(EXTRACXXFLAGS)
# folder to hold object files
BINFOLDER = bin
# name of the executable
EXECUTABLE = gzipUtil.out
# Objects needed for the executable
OBJECTS = $(patsubst %.cpp,$(BINFOLDER)/%.o,$(wildcard *.cpp))
# Dependencies of each of the object being built
DEPS = $(OBJECTS:.o=.d)

# target to build everything
all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

.PRECIOUS: $(BINFOLDER)/. $(BINFOLDER)%/.

$(BINFOLDER)/.:
	mkdir -p $@

$(BINFOLDER)%/.:
	mkdir -p $@

$(BINFOLDER)/%.o : %.cpp | $$(@D)/.
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJECTS) $(DEPS) $(EXECUTABLE)

-include $(DEPS)