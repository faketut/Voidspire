CXX=g++ 
CXXFLAGS=-std=c++20 -Wall -O0 -g -MMD -Werror=vla # use -MMD to generate dependencies
SOURCES=main.cc  # list of all .cc files in the current directory
OBJECTS=${SOURCES:.cc=.o}  # .o files depend upon .cc files with same names
DEPENDS=${OBJECTS:.o=.d}   # .d file is list of dependencies for corresponding .cc file
EXEC=cc3k

# First target in the makefile is the default target.
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.cc 
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

-include ${DEPENDS}

.PHONY: clean
clean:
	rm  -f $(OBJECTS) $(DEPENDS) $(EXEC)
