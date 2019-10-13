#
# Makefile that is generic enough to work for all targets
# 
# Here we need to specify the TARGET as an argument when invoking make
#
# Examples: make TARGET=prog1
#           make clean TARGET=prog2
#           make TARGET=prog3
#

# Specify the compiler: g++ for C++
CXX = g++

# C++ compiler flags
# -Wall     turns on most, but not all warnings
# -O1       Optimization level 1
# -fopenmp  Compile with OpenMP for shared memory parallel  
CXXFLAGS = -Wall

# Include path for Eigen
INC=-I/usr/include/eigen3

all: $(TARGET)
	@echo 'argument is '$(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) $(INC) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
