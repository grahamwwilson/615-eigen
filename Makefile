#
# Makefile using pattern rules.
# Here for each program in the list of programs, the .cpp file 
# is compiled with the commands/options specified by 
# $(CXX), $(CXXFLAGS) and $(INC).
# See p16 of "Managing projects with GNU make" by Mecklenburg for 
# more details, particularly the automatic variables
# 

# Specify the compiler: g++ for C++
CXX = g++

# C++ compiler flags
# -Wall     turns on most, but not all warnings
# -O1       Optimization level 1
# -fopenmp  Compile with OpenMP for shared memory parallel  
CXXFLAGS = `root-config --cflags --glibs`

# Include path for Eigen
INC=-I/usr/include/eigen3

programs = prog1 prog2 prog3 Example_SVD Example_LLSQ Example_LLSQB
all: $(programs)

$(programs): %: %.cpp
	$(CXX) $(INC) -o $@ $< $(CXXFLAGS)

clean:
	$(RM) $(programs)
