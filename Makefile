CXX = g++
CXXFLAGS = -std=c++11

all: serverM serverS serverL serverH client

serverM: serverM.cpp
	$(CXX) $(CXXFLAGS) -o serverM serverM.cpp

serverS: serverS.cpp
	$(CXX) $(CXXFLAGS) -o serverS serverS.cpp

serverL: serverL.cpp
	$(CXX) $(CXXFLAGS) -o serverL serverL.cpp

serverH: serverH.cpp
	$(CXX) $(CXXFLAGS) -o serverH serverH.cpp

client: client.cpp build_encryption_map.h
	$(CXX) $(CXXFLAGS) -o client client.cpp

clean:
	rm -f serverM serverS serverL serverH client
