CXX = g++ -O3
CXXFLAGS = -std=c++14

oglflags = -lsfml-graphics -lsfml-window -lsfml-system -lpthread

default: perlin.o main.o 
	$(CXX) ${CXXFLAGS} -o defaultOut main.o perlin.o ${oglflags} 

debug: 
	$(CXX) ${CXXFLAGS} -g -Wall -o debugOut main.cpp perlin.cpp ${oglflags}

%.o: %.cpp %.h
	$(CXX) ${CXXFLAGS} -c $< $(oglflags)

clean:
	rm -rf *.o ./defaultOut ./debugOut *.gch
