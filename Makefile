################################################################################
#
#
#  Universidade Federal Rural do Rio de Janeiro
#  Instituto Multidisciplinar - Departamento de Ciencia da Computacao
#  por: Marcelo Zamith - zamith.marcelo@gmail.com
#  para compilar com target release use o comando: make target=release + rápido
#
#  versão de on-ramp
#
# Options:
# make CA_GL compiles all project without dynamic lib
# make MyLib compiles only CA lib
# make CA_GL_SO compiles projkect using CA in so lib
################################################################################
VERSION      = -D_VERSION=\"0.1\"
APPLICATION  = -D_APPLICATION=\"$(EXEFILE)\"
CPUCPP    = g++
CPPFLAGS  = --std=c++11 -m64 -Wno-reorder #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
CFLAGS    = --std=c11 -m64	 #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS      = $(APPLICATION)  $(VERSION) -DALIGN=64
INCLUDES  =	-I.
LIBDIR   = -L/usr/lib -L/opt/glew/lib -L.
LIBS     =    -lm
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE_CPP = $(CPUCPP) $(DEFS) $(INCLUDES) $(CPPFLAGS)

ifeq ($(target), release)
  DEFS      +=
  CPPFLAGS  += -Ofast
else
  DEFS     += -D_DEBUG
  CPPFLAGS += -g -Wall -O0
endif

SIMU:	main CACityExpansion
		$(CPU_COMPILE_CPP)  obj/CACityExpansion.o obj/main.o  $(LINK) -o bin/ca-city-simulation

CA-GL:	main-glut CACityExpansion
	$(CPU_COMPILE_CPP)  obj/CACityExpansion.o obj/main-glut.o  $(LINK)  -lGLEW -lGLU -lglut -lGL -o bin/CA-GL

#CA_GL_SO: MyLib main-glut
#	$(CPU_COMPILE_CPP) main-glut.o  $(LINK) -lCityExpansion.v.0.0  -lGLEW -lGLU -lglut -lGL -o CA-GL-SO

#MyLib: LibCellularAutomata
#	$(CPU_COMPILE_CPP) -shared -fPIC -o libCityExpansion.v.0.0.so LibCellularAutomata.o LibGameOfLife.o


#Compile full code
main-glut:
	$(CPU_COMPILE_CPP) -c main-glut.cpp -o obj/main-glut.o

main:
		$(CPU_COMPILE_CPP) -c main.cpp -o obj/main.o

CACityExpansion:
		$(CPU_COMPILE_CPP) -c CACityExpansion.cpp -o obj/CACityExpansion.o


#Create lib only CA rules
LibCellularAutomata:
		$(CPU_COMPILE_CPP) -fPIC  -c CACityExpansion.cpp -o LibCellulCACityExpansionarAutomata.o


clean:
	rm obj/*.o; rm CA-GL; rm libCityExpansion.v.0.0.so; rm CA-GL-SO

files:
	rm *.txt; rm *.dat;
