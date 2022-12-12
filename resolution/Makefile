# Makefile

all: solver

res:
	gcc -g -o main resolution.c import_export.c

import_export:
	gcc -Wall -Wextra -Werror import_export.c -o import_export

solver:
	gcc -g -Wall -Wextra -Werror -O3 -C -Iinclude import_export.c \
		resolution.c -o solver

clean:
	rm -f *.d
	rm -f *.o
	rm -f *.result
	rm -f import_export resolution solver

# END Makefile
