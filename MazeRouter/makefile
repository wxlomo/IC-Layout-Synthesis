#####################################
#   ECE1387H Assignment 1           #
#   Weixuan Yang s100865594         #
#   weixuan.yang@mail.utoronto.ca   #
#####################################

PLATFORM = X11
#PLATFORM = WIN32
#PLATFORM = NO_GRAPHICS

HDR = segment.h router.h plot.h graphics.h easygl_constants.h 
SRC = main.cpp graphics.cpp
EXE = maze_router
BACKUP_FILENAME=`date "+backup-%Y%m%d-%H%M.zip"`
FLAGS = -g -Wall -D$(PLATFORM)

# Need to tell the linker to link to the X11 libraries.
# WIN32 automatically links to the win32 API libraries (no need for flags)
ifeq ($(PLATFORM),X11)
   GRAPHICS_LIBS = -lX11
endif

$(EXE): graphics.o main.o
	g++ $(FLAGS) graphics.o main.o $(GRAPHICS_LIBS) -o $(EXE)

graphics.o: graphics.cpp $(HDR)
	g++ -c $(FLAGS) graphics.cpp

main.o: main.cpp $(HDR)
	g++ -c $(FLAGS) main.cpp