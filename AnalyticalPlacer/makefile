#   An Analytical Placer     
#   Weixuan Yang    
#   weixuanyang@outlook.com 
 
PLATFORM = X11
GRAPHICS_LIBS = -lX11
FLAGS = -Wall -D$(PLATFORM)

OBJ = main.o placer.o graphics.o segment.o
TGT = place

all: $(TGT)

$(TGT): $(OBJ) placer.o
	g++ $(FLAGS) $(OBJ) $(GRAPHICS_LIBS) libumfpack.a libamd.a libsuitesparseconfig.a -o $(TGT)

clean:
	rm -f $(OBJ) $(TGT)