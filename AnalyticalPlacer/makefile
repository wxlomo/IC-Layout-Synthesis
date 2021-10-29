#   ECE1387H Assignment 2          
#   Weixuan Yang s100865594        
#   weixuan.yang@mail.utoronto.ca 
 
PLATFORM = X11
GRAPHICS_LIBS = -lX11
FLAGS = -Wall -D$(PLATFORM)

OBJ = main.o graphics.o mgmres.o placer.o segment.o
TGT = placer

all: $(TGT)

$(TGT): $(OBJ)
	g++ $(FLAGS) $(OBJ) $(GRAPHICS_LIBS) -o $(TGT)

clean:
	rm -f $(OBJ) $(TGT)