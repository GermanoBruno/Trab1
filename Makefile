all: main.o veiculos.o linhas.o
	gcc main.o veiculos.o linhas.o -o program
main.o: main.c
	gcc main.c -c
veiculos.o: veiculos.c
	gcc veiculos.c -c
linhas.o: linhas.c
	gcc linhas.c -c
run:
	./program
clean:
	rm -rf program *.o testes/teste*
test:
	./program < testes/caso1.in > testes/teste1.out
	diff testes/teste1.out testes/caso1.out
	./program < testes/caso2.in > testes/teste2.out
	diff testes/teste2.out testes/caso2.out