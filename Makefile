# having the option -DNDEBUG when compiling, means NO debugging occurs
# comment out the next line if you want debugging
NO_DEBUGGING_STATEMENTS = -DNDEBUG
CXX = g++
VERSION = -std=c++14
#DEBUG = -g
CFLAGS = -pedantic -Wall -Wextra $(VERSION) $(DO_NOT_DEBUG)
LFLAGS = -Wall $(VERSION) 

SRCS = encoder_decoder.cpp

OBJS = $(SRCS:.cpp=.o)
EXEC = encoder_decoder

all: $(SRCS) $(EXEC)

# To make an object from source
#.cpp.o: 
%.o:%.cpp $(INCS)
	$(CXX) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS) 
	$(CXX) $(LFLAGS) $(OBJS) -o $@

# for Windows
clean:
	del *.o  $(EXEC).exe
	@echo clean done

# for UNIX / Linux 
remove:
	\rm -f *.o  $(EXEC)
	@echo clean done