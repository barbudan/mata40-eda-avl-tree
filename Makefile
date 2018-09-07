arquivo: main.o avl.o
	@echo Criando executável
	gcc avl.o main.o -o arquivo -lm -Wall -Wextra -Werror -Wpedantic

main.o: ./src/main.c
	@echo Criando main
	gcc -c main.c -lm -Wall -Wextra -Werror -Wpedantic

avl.o: ./src/avl.c ./lib/avl.h
	@echo Criando avl
	gcc -c avl.c -lm -Wall -Wextra -Werror -Wpedantic