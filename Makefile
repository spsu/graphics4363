# =======================================
# Makefile Setup
# I *really* need to learn the Autotools
# =======================================

C = g++ -O0 -g -Wall -fPIC 
LN = g++ -O0 -g -lstdc++
SHARED = g++ -shared -lstdc++
CD = cd
RM = /bin/rm -f
INC = -I/usr/local/include -I/usr/include/GL
LIB = -lglut -lGL -lGLU -lGLEW -lgltools
#INC = `pkg-config --cflags-only-I ......` #
#LIB = `pkg-config --libs ...... ` #


# ================================
# Helpful Targets for any project
# ================================

all: 
	@echo 'Cannot compile all programs at once.'
	@echo '"make \tab" to see targets.'

.PHONY: clean
clean: 
	$(RM) main test project1 project2 two
	$(RM) *.o *.a *.so *.out
	cd ./build && $(RM) *.o *.so
	cd ./build && $(RM) */*.o */*.so */*/*.o */*/*.so

.PHONY: stats
stats:
	@echo " Lines\tWords\tBytes\t"
	@wc -l -w -c */*.*pp 
	@echo " Lines\tWords\tBytes\t"


# ==============
# Build Targets 
# ==============

### PROJECT ONE #############################
project1: source/project1.cpp required 
	@echo "[compile] project one"
	@$(CD) ./build && $(C) $(INC) -c ../source/project1.cpp
	@echo "[link] linking ALL object files" 
	@$(RM) build/project*.o
	@$(LN) $(LIB) build/*.o build/libs/*.o -o project1 
	@chmod +x project1 
	@echo "\nBuild Success!\n"

### PROJECT TWO ######################
two: source/project2.cpp required 
	@echo "[compile] project two"
	@$(CD) ./build && $(C) $(INC) -c ../source/project2.cpp
	@echo "[link] linking ALL object files" 
	@$(RM) build/project1.o
	@$(LN) $(LIB) build/*.o build/libs/*.o build/geometry/*.o -o two 
	@chmod +x two 
	@echo "\nBuild Success!\n"


### ALL LIBS ##########################
required: \
	build/libs
		@$(CD) .

# Normally you don't maintain something like this by hand...
# ==========================================================

build/libs: \
	build/libs/math.o \
	build/libs/shaders.o \
	build/libs/vertex.o \
	build/libs/vao.o \
	build/geometry/sphere.o
		@$(CD) . 

build/libs/math.o: source/libs/math.hpp source/libs/math.cpp
	@echo "[compile] math lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/math.cpp

build/libs/shaders.o: source/libs/shaders.hpp source/libs/shaders.cpp
	@echo "[compile] shaders lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/shaders.cpp

build/libs/vertex.o: source/libs/vertex.hpp source/libs/vertex.cpp
	@echo "[compile] vertex lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/vertex.cpp

build/libs/vao.o: source/libs/vao.hpp source/libs/vao.cpp
	@echo "[compile] vao lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/vao.cpp

build/geometry/sphere.o: source/geometry/sphere.hpp source/geometry/sphere.cpp
	@echo "[compile] sphere lib"
	@$(CD) ./build/geometry && $(C) $(INC) -c ../../source/geometry/sphere.cpp


