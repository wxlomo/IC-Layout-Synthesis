#   A MAX-SAT Solver  
#   Weixuan Yang    
#   weixuanyang@outlook.com
 
PLATFORM = X11
GRAPHICS_LIBS = -lX11
FLAGS = -Wall -D$(PLATFORM)

OBJ = main.o maxsat.o graphics.o segment.o
TGT = maxsat

all: $(TGT)

$(TGT): $(OBJ)
	g++ $(FLAGS) $(OBJ) $(GRAPHICS_LIBS) -o $(TGT)

clean:
	rm -f $(OBJ) $(TGT)
