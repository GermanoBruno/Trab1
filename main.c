#include <stdio.h>
#include <stdlib.h>
#include "veiculo.h"
#include "auxiliares.h"
/*
#include <stdlib.h>
#include <string.h>
#include "linhas.h"
#include "veiculo.h"
#include "auxiliares.h"*/

/*FILE* fp = fopen("veiculo.bin", "rb");
	HeaderVeiculo* h = criarHeader();
	

	readHeaderBin(h, fp);
	imprimeHeader(h);

	RegistroVeiculo* v = create();
	char removido; int tam;

	for (int i = 0; i < 5; i++){
	
		leituraBinario(fp, v, &removido, &tam);
		imprimeVeiculo(v);
		printf("%d\n", getPrefix(v));
	
	}
	fclose(fp);
	liberaHeader(h);
	libera(v);*/


int main(int argc, char const *argv[])
{

	FILE* filevehicle = fopen("veiculo.bin", "rb");
	FILE* filebtree = fopen("btree.bin", "wb+");
	
	HeaderVeiculo* h = criarHeader();
	page* root = NULL;
	int rrn, tam; char removido;

	writeHeader(filebtree, '0', 0, 1);

	readHeaderBin(h, filevehicle);

	int prefix;
	for (int i = 0; i < 2; i++) {

		int pos = ftell(filevehicle);
		RegistroVeiculo* v = create();
		
		leituraBinario(filevehicle, v, &removido, &tam);
		prefix = getPrefix(v);
		
		imprimeVeiculo(v);
		printf("%d %d %d\n", pos, tam, prefix);
		
		btreeInsert(filebtree, &root, prefix, pos, &rrn);
		libera(v);

	}

	//if (searchKey(filebtree, getRRN(root), prefix)) printf("Achei o %d\n", prefix);
	//else printf("Nao achei\n");

	free(root); liberaHeader(h); 
	fclose(filebtree); fclose(filevehicle);

	return 0;
}

/*
	FILE* bin = fopen("btree.bin", "wb+");
	page* root = NULL;
	page* test = createPage();
	int rrn;

	writeHeader(bin, '0', 0, 0);

	btreeInsert(bin, &root, 1, 0, &rrn);
	btreeInsert(bin, &root, 2, 1, &rrn);
	btreeInsert(bin, &root, 3, 2, &rrn);
	btreeInsert(bin, &root, 4, 3, &rrn);
	btreeInsert(bin, &root, 5, 4, &rrn);
	btreeInsert(bin, &root, 6, 5, &rrn);
	btreeInsert(bin, &root, 7, 6, &rrn);
	btreeInsert(bin, &root, 8, 7, &rrn);

	//printf("%ld\n",ftell(bin));
	//int key = 9;
	//if (searchKey(bin, getRRN(root), key)) printf("Achei o %d\n", key);
	//else printf("Nao achei\n");

	free(test);
	free(root);
	fclose(bin);*/

/* ---------------- EXTRA ----------------

OPCIONAL: dicas sobre scanf() e fscanf():

scanf("%[^,]", string) -> lê até encontrar o caractere ',', não incluindo o mesmo na leitura.

Exemplo de entrada: "Oi, esse é um exemplo."
Nesse caso, o scanf("%[^,]") tem como resultado a string "Oi";

scanf("%[^\"]", string) -> lê até encontrar o caractere '"', não incluindo o mesmo na leitura.
scanf("%[^\n]", string) -> lê até encontrar o fim da linha, não incluindo o '\n' na leitura.

scanf("%*c") --> lê um char e não guarda em nenhuma variável, como se tivesse ignorado ele

*/
