C_SOURCES = $(wildcard linalg/matrix.c img/img.c *.c)
HEADERS = $(wildcard linalg/matrix.h img/img.h library.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS =

MAIN = main
CC = /usr/bin/gcc
LINKER = /usr/bin/ld

run: ${MAIN}
	./${MAIN}

main: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ -lm

# Generic rules
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@ -lm

clean:
	rm matrix/*.o *.o img/*.o ./*.o ${MAIN}