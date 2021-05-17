all: main.o livrariax.o
	gcc main.o livrariax.o -o program
main.o: main.c
	gcc main.c -c
livrariax.o: livrariax.c
	gcc livrariax.c -c
run:
	./program
clean:
	rm -rf program *.o testes/teste*
test:
	./program < testes/caso1.in > testes/teste1.out
	diff testes/teste1.out testes/caso1.out
	./program < testes/caso2.in > testes/teste2.out
	diff testes/teste2.out testes/caso2.out