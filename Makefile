PROG     = clonefish

#SRCS     = $(wildcard *.cpp)
SRCS     = main.cpp

OBJS     = $(SRCS:.cpp=.o)
CXXFLAGS = -O2 -Wall -ansi # -Werror to force warnings as errors
LDLIBS   = 
INCLUDE  =
DEPFILE  = deps.mak

.PHONY: clean

$(PROG): $(OBJS)
	$(CXX) $^ $(LDLIBS) -o $@

# How to make the object files:
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

deps:
	$(CXX) -MM $(CXXFLAGS) $(SRCS) > $(DEPFILE)

# Cleaning target (only works with fileutils):
clean:
	$(RM) $(OBJS) $(PROG)

-include $(DEPFILE)
