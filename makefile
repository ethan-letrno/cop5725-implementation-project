CXX = g++
CXXFLAGS = -std=c++11

tds : utils.o divide_subsets.o partition_children.o tds.o build_preliminary_solution.o main.o
	$(CXX) $(CXXFLAGS) -o tds utils.o divide_subsets.o partition_children.o tds.o build_preliminary_solution.o main.o
	
main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

build_preliminary_solution.o : build_preliminary_solution.cpp
	$(CXX) $(CXXFLAGS) -c build_preliminary_solution.cpp
	
tds.o : tds.cpp
	$(CXX) $(CXXFLAGS) -c tds.cpp

partition_children.o : partition_children.cpp
	$(CXX) $(CXXFLAGS) -c partition_children.cpp

divide_subsets.o : divide_subsets.cpp
	$(CXX) $(CXXFLAGS) -c divide_subsets.cpp

utils.o : utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp
	
clean:
	rm tds *.o
