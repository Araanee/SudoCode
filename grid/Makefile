# Makefile

all: main

grid_detection:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" grid_detection.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o grid_detection -D__NO_INLINE__

grid_extraction:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" grid_row_extraction.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o grid_row_extraction -D__NO_INLINE__

grid_numbers_extraction:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" grid_numbers_extraction.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o grid_numbers_extraction -D__NO_INLINE__

main:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" grid_detection.c grid_row_extraction.c grid_numbers_extraction.c main.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o main -D__NO_INLINE__

clean_image:
	rm -f c_image.jpeg
	rm -f 1.jpeg
	rm -f 2.jpeg
	rm -f 3.jpeg
	rm -f 4.jpeg
	rm -f 5.jpeg
	rm -f 6.jpeg
	rm -f 7.jpeg
	rm -f 8.jpeg
	rm -f 9.jpeg
	rm -f numbers/*.jpeg

clean:
	rm -f *.d
	rm -f *.o
	rm -f c_image.jpeg
	rm -f 1.jpeg
	rm -f 2.jpeg
	rm -f 3.jpeg
	rm -f 4.jpeg
	rm -f 5.jpeg
	rm -f 6.jpeg
	rm -f 7.jpeg
	rm -f 8.jpeg
	rm -f 9.jpeg
	rm -f numbers/*.jpeg
	rm -f grid_detection
	rm -f main

# END Makefile
