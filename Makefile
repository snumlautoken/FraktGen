LDFLAGS= -lglfw -lGLU -lGL -lGLEW
CXX=g++
all: FraktGen
FraktGen: src/*.cpp
	    $(CXX) $^ -o $@ $(LDFLAGS)
