#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "veiculo.h"

#define tamveiculos 1

int main(void) {

	FILE *fp, *binario;
	header* h = createHeader();
	
	//fp = fopen("veiculo.csv", "r");
	fp = fopen("teste.csv", "r");
	descreveHeader(h, fp);
	binario = fopen("veiculo.bin", "wb");
	setHeader(h, binario);
	
	int i = 0;
	while (i < tamveiculos) { 	// Fazer função pra ler até o fim do arquivo depois
		
		veiculo* v = create();
		leitura(fp, v);
		atualizaBinario(h, v, binario);
		libera(v);
		i++;

	}
	
	//binarioNaTela("veiculo.bin");
	//imprime(v);
	
	fclose(fp);
	fclose(binario);

	return 0;
}