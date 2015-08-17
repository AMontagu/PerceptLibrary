CXX ?=g++
CXXFLAGS += -std=c++11 -c -Wall $(shell pkg-config --cflags opencv pocketsphinx sphinxbase)
LDFLAGS += $(shell pkg-config --libs opencv pocketsphinx sphinxbase)
EXEC=RobotLabLibrary

all: $(EXEC)

RobotLabLibrary: main.o src/Voice.o src/Video.o src/Recognizer.o faceRecognizerContrib/src/eigen_faces.o faceRecognizerContrib/src/facerec.o faceRecognizerContrib/src/fisher_faces.o faceRecognizerContrib/src/lbph_faces.o
	$(CXX) -o $@ $^ $(LDFLAGS)

eigen_faces.o: faceRecognizerContrib/src/precomp.hpp faceRecognizerContrib/src/face_basic.hpp 
	
facerec.o: faceRecognizerContrib/src/precomp.hpp faceRecognizerContrib/include/face.hpp

fisher_faces.o: faceRecognizerContrib/src/precomp.hpp faceRecognizerContrib/src/face_basic.hpp 

lbph_faces.o: faceRecognizerContrib/src/precomp.hpp faceRecognizerContrib/src/face_basic.hpp faceRecognizerContrib/include/face.hpp

Recognizer.o: faceRecognizerContrib/include/face.hpp include/Constantes.h

Video.o: include/Constantes.h

Voice.o: include/Constantes.h

main.o: include/Video.h include/Recognizer.h include/Voice.h

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
