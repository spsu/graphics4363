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
	$(RM) main test project1
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

### MAIN #############################
p1: source/project1.cpp required 
	@echo "[compile] project one"
	@$(CD) ./build && $(C) $(INC) -c ../source/project1.cpp
	@echo "[link] linking ALL object files" 
	$(LN) $(LIB) build/*.o build/libs/*.o -o project1 
	@chmod +x project1 
	@echo "\nBuild Success!\n"

### ALL LIBS ##########################
required: \
	build/libs
		@$(CD) .

# Normally you don't maintain something like this by hand...
# ==========================================================

build/libs: \
	build/libs/math.o \
	build/libs/misc.o
		@$(CD) . 

build/libs/math.o: source/libs/math.hpp source/libs/math.cpp
	@echo "[compile] math lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/math.cpp

build/libs/misc.o: source/libs/misc.hpp source/libs/misc.cpp
	@echo "[compile] misc lib"
	@$(CD) ./build/libs && $(C) $(INC) -c ../../source/libs/misc.cpp

