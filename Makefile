SHELL = /bin/sh

SRC = src/main.c src/grid.c src/mask.c
# ${SRC:.c=.o} replaces all the .c extensions in ${SRC} with .o
OBJ = ${SRC:.c=.o}
DEPS = 
CFLAGS = -Wall -Werror -g
CC = clang
INCLUDE = 
LIBS = -lm
OUT = fresnel

# $@ is the name of the task (in this case ${OUT})
# $^ is the files passed after the colon, in this case all the object files
${OUT}: ${OBJ}
	${CC} ${CFLAGS} ${INCLUDE} -o $@ $^ ${LIBS}

# $< is the first file passed after the colon
# We don't actually pass the header files to gcc, but we want the target to be out of date when
# we've changed them, hence why we specify them as a dependency here
%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

# .PHONY tells make that this target is just a name and not a file (i.e. there is no file called clean)
# It also means that this target is always 'out of date' (i.e. will always run when asked to)
.PHONY: clean
clean:
	rm -rf *.o ${OUT}
