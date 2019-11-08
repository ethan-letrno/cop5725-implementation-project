CXX = g++
CXXFLAGS = -std=c++11

naive : naive.o
	$(CXX) $(CXXFLAGS) -o naive naive.o
	
naive.o : naive.cpp
	$(CXX) $(CXXFLAGS) -c naive.cpp
	
clean:
	rm naive.o naive