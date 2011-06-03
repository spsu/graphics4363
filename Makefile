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
	$(LN) $(LIB) build/*.o -o project1 
	@chmod +x project1 
	@echo "\nBuild Success!\n"

# Alias
project1: p1
	@$(CD) .

### ALL LIBS ##########################
required: \
	build/libs.o
		@$(CD) .

# Normally you don't maintain something like this by hand...
# ==========================================================

build/libs.o: source/libs.hpp source/libs.cpp
	@echo "[compile] libs"
	$(CD) ./build && $(C) $(INC) -c ../source/libs.cpp


