CXX = g++
CXXFLAGS = -I. -std=c++11

my_program: main.o W_FrREST.o
	$(CXX) -o my_program main.o W_FrREST.o

main.o: main.cpp W_FrREST.h json.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

W_FrREST.o: W_FrREST.cpp W_FrREST.h json.hpp
	$(CXX) $(CXXFLAGS) -c W_FrREST.cpp

clean:
	rm -f *.o my_program
