#
# Makefile using pattern rules
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

programs = prog1 prog2 prog3 Example_SVD
all: $(programs)

$(programs): %: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -o $@ $<

clean:
	$(RM) $(programs)
