CXX ?=g++
CXXFLAGS += -std=c++11 -c -Wall $(shell pkg-config --cflags opencv)
LDFLAGS += $(shell pkg-config --libs --static opencv)
EXEC=RobotLabLibrary

all: $(EXEC)

RobotLabLibrary: Video.o main.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	
Video.o: Constantes.h

main.o: Video.h

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)