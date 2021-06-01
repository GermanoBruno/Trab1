all: main.o veiculo.o linhas.o auxiliares.o
	gcc main.o veiculo.o linhas.o auxiliares.o -o program
main.o: main.c
	gcc main.c -c
veiculos.o: veiculo.c
	gcc veiculo.c -c
auxiliares.o: auxiliares.c
	gcc auxiliares.c -c
linhas.o: linhas.c
	gcc linhas.c -c
run:
	./program
clean:
	rm -rf program *.o testes/teste* Trab1.zip
valg:
	valgrind --leak-check=full --track-origins=yes ./program
zip:
	zip Trab1 linhas.* main.c Makefile veiculo.c veiculo.h auxiliares.*
test1:
	./program < testes/1.in > testes/teste1.out 
	diff testes/teste1.out testes/1.out
test2: 
	./program < testes/2.in > testes/teste2.out 
	diff testes/teste2.out testes/2.out
test3: 
	./program < testes/3.in > testes/teste3.out 
	diff testes/teste3.out testes/3.out
test4: 
	./program < testes/4.in > testes/teste4.out 
	diff testes/teste4.out testes/4.out
test5: 
	./program < testes/5.in > testes/teste5.out 
	diff testes/teste5.out testes/5.out
test6: 
	./program < testes/6.in > testes/teste6.out 
	diff testes/teste6.out testes/6.out
test7: 
	./program < testes/7.in > testes/teste7.out 
	diff testes/teste7.out testes/7.out
test8: 
	./program < testes/8.in > testes/teste8.out 
	diff testes/teste8.out testes/8.out
test9: 
	./program < testes/9.in > testes/teste9.out 
	diff testes/teste9.out testes/9.out
test10: 
	./program < testes/10.in > testes/teste10.out 
	diff testes/teste10.out testes/10.out
test11: 
	./program < testes/11.in > testes/teste11.out 
	diff testes/teste11.out testes/11.out
test12: 
	./program < testes/12.in > testes/teste12.out 
	diff testes/teste12.out testes/12.out
test13: 
	./program < testes/13.in > testes/teste13.out 
	diff testes/teste13.out testes/13.out
test14: 
	./program < testes/14.in > testes/teste14.out 
	diff testes/teste14.out testes/14.out
test15: 
	./program < testes/15.in > testes/teste15.out 
	diff testes/teste15.out testes/15.out
test16: 
	./program < testes/16.in > testes/teste16.out 
	diff testes/teste16.out testes/16.out
test:
	./program < testes/1.in > testes/teste1.out 
	diff testes/teste1.out testes/1.out 
	./program < testes/2.in > testes/teste2.out 
	diff testes/teste2.out testes/2.out 
	./program < testes/3.in > testes/teste3.out 
	diff testes/teste3.out testes/3.out 
	./program < testes/4.in > testes/teste4.out 
	diff testes/teste4.out testes/4.out 
	./program < testes/5.in > testes/teste5.out 
	diff testes/teste5.out testes/5.out 
	./program < testes/6.in > testes/teste6.out 
	diff testes/teste6.out testes/6.out 
	./program < testes/7.in > testes/teste7.out 
	diff testes/teste7.out testes/7.out 
	./program < testes/8.in > testes/teste8.out 
	diff testes/teste8.out testes/8.out 
	./program < testes/9.in > testes/teste9.out 
	diff testes/teste9.out testes/9.out 
	./program < testes/10.in > testes/teste10.out 
	diff testes/teste10.out testes/10.out 
	./program < testes/11.in > testes/teste11.out 
	diff testes/teste11.out testes/11.out 
	./program < testes/12.in > testes/teste12.out 
	diff testes/teste12.out testes/12.out 
	./program < testes/13.in > testes/teste13.out 
	diff testes/teste13.out testes/13.out 
	./program < testes/14.in > testes/teste14.out 
	diff testes/teste14.out testes/14.out 
	./program < testes/15.in > testes/teste15.out 
	diff testes/teste15.out testes/15.out 
	./program < testes/16.in > testes/teste16.out 
	diff testes/teste16.out testes/16.out