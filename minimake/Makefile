NEED = make.c execute.c parse.c print.c
CC = gcc -Wall -Wextra -fsanitize=address -g

all: ${NEED}
	$(CC) ${NEED} -o minimake

clean:
	${RM} -r minimake