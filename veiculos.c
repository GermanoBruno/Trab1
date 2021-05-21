#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "veiculo.h"

struct Header {

	char status;
	long int byteProxReg;
	int nroRegistros;
	int nroRegistrosRemovidos;
	char descrevePrefixo[19];
	char descreveData[36];
	char descreveLugares[43];
	char descreveLinha[27];
	char descreveModelo[18];
	char descreveCategoria[21];

};

struct Veiculo {

	char prefixo[6];
	char data[11];
	int quatidadeLugares;
	int codLinha;
	char* modelo;
	char* categoria;
	char removido;
	int tamanhoRegistro;
	int tamanhoModelo;
	int tamanhoCategoria;



};

header* createHeader() {

	header* h = (header*)malloc(sizeof(header));
	h->status = '0';
	h->byteProxReg = 0;
	h->nroRegistros = 0;
	h->nroRegistrosRemovidos = 0;

}

void descreveHeader(header* h, FILE* fp) {

	fscanf(fp, "%[^,],", h->descrevePrefixo);
	fscanf(fp, "%[^,],", h->descreveData);
	fscanf(fp, "%[^,],", h->descreveLugares);
	fscanf(fp, "%[^,],", h->descreveLinha);
	fscanf(fp, "%[^,],", h->descreveModelo);
	fscanf(fp, "%[^\n]\n", h->descreveCategoria);
	
/*	printf("%s\n", h->descrevePrefixo);
	printf("%s\n", h->descreveData);
	printf("%s\n", h->descreveLugares);
	printf("%s\n", h->descreveLinha);
	printf("%s\n", h->descreveModelo);
	printf("%s\n", h->descreveCategoria);
*/

}

void setHeader(header* h, FILE* binario) {

	fwrite(&(h->status), sizeof(char), 1, binario);
	fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
	fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
	fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
	fwrite(h->descrevePrefixo, sizeof(char), 18, binario);
	fwrite(h->descreveData, sizeof(char), 35, binario);
	fwrite(h->descreveLugares, sizeof(char), 42, binario);
	fwrite(h->descreveLinha, sizeof(char), 26, binario);
	fwrite(h->descreveModelo, sizeof(char), 17, binario);
	fwrite(h->descreveCategoria, sizeof(char), 20, binario);
	h->byteProxReg = 175;

}

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

int escrita (veiculo* v, char removido, int byteProxReg, FILE* binario) {

	int tamodelo = strlen(v->modelo);
	int tamcategoria = strlen(v->categoria);
	int tam = 23 + tamodelo + tamcategoria;

	fseek(binario, byteProxReg, SEEK_SET);

	fwrite(&(removido), sizeof(char), 1, binario);
	fwrite(&(tam), sizeof(int), 1, binario);
	fwrite(v->prefixo, sizeof(char), strlen(v->prefixo), binario);
	fwrite(v->data, sizeof(char), strlen(v->data), binario);
	fwrite(&(v->quatidadeLugares), sizeof(int), 1, binario);
	fwrite(&(v->codLinha), sizeof(int), 1, binario);
	fwrite(&(tamodelo), sizeof(int), 1, binario);
	fwrite(v->modelo, sizeof(char), strlen(v->modelo), binario);
	fwrite(&(tamcategoria), sizeof(int), 1, binario);
	fwrite(v->categoria, sizeof(char), strlen(v->categoria), binario);
	int total = 13 + tam;

	return total;
}

// LEMBRAR DE CRIAR FUNCAO PARA ATUALIZAR O STATUS

void atualizaBinario(header* h, veiculo* v, FILE* binario) {

	// -----------FALTA ESCREVER AS MUDANÇAS NO HEADER -------------

	int tam;
	char removido;
	//int tam = 23 + tamodelo + tamcategoria; // LEMBRAR QUE O DEFINITIVO É COM CAMPOS NULOS

	if (v->prefixo[0] == '*') {
		
		h->nroRegistrosRemovidos++;
		removido = '0'; 
	}
	else  {

		h->nroRegistros++;
		removido = '1';

	}

	tam = escrita(v, removido, h->byteProxReg, binario);

	h->byteProxReg = h->byteProxReg + tam;

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