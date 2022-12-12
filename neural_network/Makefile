# Makefile

all: neural_network

neural_network:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude neural_network.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o neural_network -lm -ldl -D__NO_INLINE__

clean:
	rm -f *.d
	rm -f *.o
	rm -f neural_network

# END Makefile
