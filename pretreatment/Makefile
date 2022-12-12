# Makefile

all: main

auto_rotation:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" rotation.c auto_rotation.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o auto_rotation -lm -ldl -D__NO_INLINE__

contrasting:
	gcc -Wall -Wextra -03 -C -Iinclude "../utils/auxiliary.c" contrasting.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o contrasting -lm -ldl -D__NO_INLINE__

delete_color:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" delete_color.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o delete_color -lm -ldl -D__NO_INLINE__

gamma:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" gamma.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o gamma -lm -ldl -D__NO_INLINE__

gaussian:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" gaussian.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o gaussian -lm -ldl -D__NO_INLINE__

grayscale:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" grayscale.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o grayscale -lm -ldl -D__NO_INLINE__

rotation:
	gcc -Wall -Wextra -O3 -C -Iinclude "../utils/auxiliary.c" rotation.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o rotation -lm -ldl -D__NO_INLINE__

threshold:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" delete_color.c threshold.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o threshold -lm -ldl -D__NO_INLINE__

main:
	gcc -Wall -Wextra -Werror -O3 -C -Iinclude "../utils/auxiliary.c" auto_rotation.c delete_color.c gamma.c gaussian.c grayscale.c rotation.c threshold.c main.c `pkg-config --cflags sdl2 SDL2_image` `pkg-config --libs sdl2 SDL2_image` -o main -lm -ldl -D__NO_INLINE__

clean_images:
	rm -f gray_image.jpeg
	rm -f rot_image.jpeg
	rm -f gau_image.jpeg
	rm -f gam_image.jpeg
	rm -f bin_image.jpeg
	rm -f rot_image.jpeg	

clean:
	rm -f *.d
	rm -f *.o
	rm -f rotation
	rm -f contrasting
	rm -f auto_rotation
	rm -f delete_color
	rm -f gaussian
	rm -f gamma
	rm -f threshold
	rm -f main
	rm -f gray_image.jpeg
	rm -f rot_image.jpeg
	rm -f gau_image.jpeg
	rm -f gam_image.jpeg
	rm -f bin_image.jpeg
	rm -f rot_image.jpeg

# END Makefile
