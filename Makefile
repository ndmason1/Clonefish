PROG     = clonefish

SRCS     = cipher.cpp util.cpp main.cpp 

OBJS     = $(SRCS:.cpp=.o)
CXXFLAGS = -O2 -Wall -ansi
LDLIBS   = -lcrypto				# link with the openssl library
INCLUDE  =

.PHONY: clean

$(PROG): $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS)

# How to make the object files:
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< $(LDLIBS)

deps:
	$(CXX) -MM $(CXXFLAGS) $(SRCS) > $(DEPFILE)

# Cleaning target (only works with fileutils):
clean:
	$(RM) $(OBJS) $(PROG)

