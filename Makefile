# Makefile

all: main

main:
	gcc -Wall -Wextra -O3 -MMD -C -rdynamic -Iinclude "utils/auxiliary.c" \
	"pretreatment/auto_rotation.c" \
	"pretreatment/delete_color.c" \
	"pretreatment/gamma.c" \
	"pretreatment/gaussian.c" \
	"pretreatment/grayscale.c" \
	"pretreatment/rotation.c" \
	"pretreatment/threshold.c" \
	"pretreatment/resize.c" \
	"pretreatment/sobel.c" \
	"grid/grid_detection.c" \
	"grid/grid_row_extraction.c" \
	"grid/grid_numbers_extraction.c" \
	"grid_to_image/construct.c" \
	ocr_neural_network/*.c \
	resolution/*.c \
	reconstruction/*.c \
	main.c \
	`pkg-config --cflags sdl2 SDL2_image` \
	`pkg-config --libs sdl2 SDL2_image` \
	`pkg-config --cflags gtk+-3.0` \
	`pkg-config --libs gtk+-3.0` \
	-o main -lm -ldl -D__NO_INLINE__

clean_images:
	rm -f gray_image.jpeg rot_image.jpeg gau_image.jpeg gam_image.jpeg \
	bin_image.jpeg rot_image.jpeg c_image.jpeg
	rm -f numbers/*.jpeg
	rm -f 1.jpeg 2.jpeg 3.jpeg 4.jpeg 5.jpeg 6.jpeg 7.jpeg 8.jpeg 9.jpeg

clean:
	rm -f *.d
	rm -f *.o
	rm -f rotation contrasting auto_rotation delete_color gaussian gamma \
	threshold resize grid_detection grid_row_extraction sobel \
	grid_numbers_extraction main
	rm -f gray_image.jpeg rot_image.jpeg gau_image.jpeg gam_image.jpeg \
	bin_image.jpeg rot_image.jpeg c_image.jpeg \
	rm -f numbers/*.jpeg \
	rm -f 1.jpeg 2.jpeg 3.jpeg 4.jpeg 5.jpeg 6.jpeg 7.jpeg 8.jpeg 9.jpeg \
	rm -f grid_00 grid_00.result solved.bmp

# END
