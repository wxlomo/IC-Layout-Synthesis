#   A Maze Router         
#   Weixuan Yang s1008655947        
#   weixuan.yang@mail.utoronto.ca 
 
PLATFORM = X11
GRAPHICS_LIBS = -lX11
FLAGS = -Wall -D$(PLATFORM)

OBJ = main.o router.o graphics.o segment.o
TGT = maze

all: $(TGT)

$(TGT): $(OBJ)
	g++ $(FLAGS) $(OBJ) $(GRAPHICS_LIBS) -o $(TGT)

clean:
	rm -f $(OBJ) $(TGT)