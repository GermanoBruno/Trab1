#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "veiculo.h"

#define tamcampos 164

int main(void) {

	FILE *fp, *binario;
	char *linha = (char*)malloc(sizeof(char)*tamcampos);
	
	fp = fopen("veiculo.csv", "r");
	binario = fopen("veiculo.bin", "wb");
	
	fread(linha, sizeof(char), tamcampos, fp);
	
	int i = 0;
	while (i < 934) {
		
		veiculo* v = create();
		leitura(fp, v);
		escrita(v, binario);
		libera(v);
		i++;

	}

	binarioNaTela("veiculo.bin");
	//imprime(v);
	
	free(linha);
	fclose(fp);
	fclose(binario);

	return 0;
}