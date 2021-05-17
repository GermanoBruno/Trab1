all: main.o binRead.o
	gcc main.o binRead.o -o program
main.o: main.c
	gcc main.c -c
binRead.o: binRead.c
	gcc binRead.c -c
run:
	./program
clean:
	rm -rf program *.o testes/teste*
test:
	./program < testes/caso1.in > testes/teste1.out
	diff testes/teste1.out testes/caso1.out
	./program < testes/caso2.in > testes/teste2.out
	diff testes/teste2.out testes/caso2.out