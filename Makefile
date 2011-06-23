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
LIB = -lglut -lGL -lGLU -lGLEW -lgltools -l3ds
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
	@$(LN) $(LIB) build/*.o \
		build/libs/*.o \
		build/shaderlib/*.o \
		build/object/*.o \
		build/loader/*.o -o two 
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
	build/libs/file.o \
	build/libs/vertex.o \
	build/object/vao.o \
	build/object/KixorObjectLoader.o \
	build/object/Lib3dsLoader.o \
	build/object/TransformationStack.o \
	build/object/TransformationStackRegistry.o \
	build/shaderlib/compiler.o \
	build/shaderlib/registry.o \
	build/loader.o
		@$(CD) . 

build/libs/file.o: source/libs/file.hpp source/libs/file.cpp
	@echo "[compile] file lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/file.cpp

build/libs/math.o: source/libs/math.hpp source/libs/math.cpp
	@echo "[compile] math lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/math.cpp

build/libs/vertex.o: source/libs/vertex.hpp source/libs/vertex.cpp
	@echo "[compile] vertex lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/vertex.cpp

build/object/vao.o: source/object/vao.hpp source/object/vao.cpp
	@echo "[compile] vao lib"
	@$(CD) ./build/object && $(C) $(INC) -c ../../source/object/vao.cpp

build/object/Lib3dsLoader.o: source/object/Lib3dsLoader.hpp source/object/Lib3dsLoader.cpp
	@echo "[compile] Lib3dsLoader"
	@$(CD) ./build/object && $(C) $(INC) -c ../../source/object/Lib3dsLoader.cpp

build/object/KixorObjectLoader.o: source/object/KixorObjectLoader.hpp source/object/KixorObjectLoader.cpp
	@echo "[compile] KixorObjectLoader"
	@$(CD) ./build/object && $(C) $(INC) -c ../../source/object/KixorObjectLoader.cpp

build/object/TransformationStack.o: source/object/TransformationStack.hpp source/object/TransformationStack.cpp
	@echo "[compile] TransformationStack"
	@$(CD) ./build/object && $(C) $(INC) -c ../../source/object/TransformationStack.cpp

build/object/TransformationStackRegistry.o: source/object/TransformationStackRegistry.hpp source/object/TransformationStackRegistry.cpp
	@echo "[compile] TransformationStackRegistry"
	@$(CD) ./build/object && $(C) $(INC) -c ../../source/object/TransformationStackRegistry.cpp

build/shaderlib/compiler.o: source/shaderlib/compiler.hpp source/shaderlib/compiler.cpp
	@echo "[compile] shader compiler"
	@$(CD) ./build/shaderlib && $(C) $(INC) -c ../../source/shaderlib/compiler.cpp

build/shaderlib/registry.o: source/shaderlib/registry.hpp source/shaderlib/registry.cpp
	@echo "[compile] shader registry"
	@$(CD) ./build/shaderlib && $(C) $(INC) -c ../../source/shaderlib/registry.cpp


# Third party code & libraries. 
# ==========================================================

# Object loader from http://www.kixor.net/dev/objloader/ (Public Domain)
build/loader.o: source/loader/*.h source/loader/*.cpp
	@echo "[compile] loader"
	@$(CD) ./build/loader && $(C) $(INC) -c ../../source/loader/*.cpp

