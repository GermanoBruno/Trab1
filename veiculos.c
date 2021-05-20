#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "veiculo.h"

struct Header {

	char status;
	long int byteProxReg;
	int nroRegistros;
	int nroRegistrosRemovidos;
	char descrevePrefixo[18];
	char descreveData[35];
	char descreveLugares[42];
	char descreveLinha[26];
	char descreveModelo[17];
	char descreveCategoria[20];

};

struct Veiculo {

	char prefixo[6];
	char data[10];
	int quatidadeLugares;
	int codLinha;
	char* modelo;
	char* categoria;
	char removido;
	int tamanhoRegistro;
	int tamanhoModelo;
	int tamanhoCategoria;



};

veiculo* create() {

	veiculo *v = (veiculo*)malloc(sizeof(veiculo));
	return v;

}

int leitura (FILE* fp, veiculo* v) {

	//if (fscanf(fp, "%[^,],", v->prefixo) == EOF) return 0;
	fscanf(fp, "%[^,],", v->prefixo);
	fscanf(fp, "%[^,],", v->data);
	fscanf(fp, "%d,", &(v->quatidadeLugares));
	fscanf(fp, "%d,", &(v->codLinha));
	fscanf(fp, "%m[^,],", &(v->modelo));
	fscanf(fp, "%m[^\n]\n", &(v->categoria));
	
	return 1;
}

void escrita (veiculo* v, FILE* binario) {

	fseek(binario, 0, SEEK_END);
	fwrite(v->prefixo, sizeof(char), strlen(v->prefixo), binario);
	fwrite(v->data, sizeof(char), strlen(v->data), binario);
	fwrite(&(v->quatidadeLugares), sizeof(int), 1, binario);
	fwrite(&(v->codLinha), sizeof(int), 1, binario);
	fwrite(v->modelo, sizeof(char), strlen(v->modelo), binario);
	fwrite(v->categoria, sizeof(char), strlen(v->categoria), binario);

}

void imprime (veiculo* v) {

	printf("%s\n", v->prefixo);
	printf("%s\n", v->data);
	printf("%d\n", v->quatidadeLugares);
    printf("%d\n", v->codLinha);
    printf("%s\n", v->modelo);
    printf("%s\n", v->categoria);


}

void libera (veiculo* v) {

	free(v->modelo);
	free(v->categoria);
	free(v);

}

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}