# Declaration of variables
CC = g++
DEBUG_FLAG = #-g
OPTI_FLAGS = -O3
WARN_FLAGS =
INCLUDE_FLAGS = #-I/usr/include/freetype2
CC_FLAGS = -std=c++0x $(DEBUG_FLAG) $(OPTI_FLAGS) $(WARN_FLAGS) $(INCLUDE_FLAGS)

# File names
EXEC = gsrd.out
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = $(wildcard *.hpp)
LIBFLAGS = -lopencv_core -lopencv_imgproc -lopencv_imgcodecs


all: $(EXEC)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(CC_FLAGS) $(OBJECTS) -o $(EXEC) $(LIBFLAGS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
