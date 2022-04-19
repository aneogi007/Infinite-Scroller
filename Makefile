sources = \
	src/main.cpp \
	src/glstate.cpp \
	src/mesh.cpp \
	src/util.cpp \
	src/camera.cpp \
	src/scene.cpp \
	src/gl_core_3_3.c
libs = \
	-lGL \
	-lglut
outname = base_freeglut

all:
	g++ -std=c++17 $(sources) $(libs) -o $(outname)
clean:
	rm $(outname)
