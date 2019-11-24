CXX = g++
CXXFLAGS = -std=c++11

tds : main.o build_preliminary_solution.o utils.o
	$(CXX) $(CXXFLAGS) -o tds main.o build_preliminary_solution.o utils.o
	
main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

build_preliminary_solution.o : build_preliminary_solution.cpp
	$(CXX) $(CXXFLAGS) -c build_preliminary_solution.cpp

utils.o : utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp
	
clean:
	rm tds *.o
