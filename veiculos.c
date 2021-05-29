#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "veiculos.h"

struct cabecalhoVeiculo {

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

struct registroVeiculo {

	char prefixo[6];
	char data[11];
	int quantidadeLugares;
	int codLinha;
	char* modelo;
	char* categoria;
	char removido;
	int tamanhoRegistro;
	int tamanhoModelo;
	int tamanhoCategoria;

};

HeaderVeiculo* criarHeader() {

	HeaderVeiculo* h = (HeaderVeiculo*)malloc(sizeof(HeaderVeiculo));
	h->status = '0';
	h->byteProxReg = 0;
	h->nroRegistros = 0;
	h->nroRegistrosRemovidos = 0;

}

void descreveHeader(HeaderVeiculo* h, FILE* fp) {

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

void setHeader(HeaderVeiculo* h, FILE* binario) {

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

void atualizaHeader (HeaderVeiculo* h, FILE* binario) {

	fseek(binario, 0, SEEK_SET);
	h->status = '1';
	fwrite(&(h->status), sizeof(char), 1, binario);
	fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
	fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
	fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);

}

RegistroVeiculo* create() {

	RegistroVeiculo *v = (RegistroVeiculo*)malloc(sizeof(RegistroVeiculo));
	return v;

}

int leitura (FILE* fp, RegistroVeiculo* v) {

	//if (fscanf(fp, "%[^,],", v->prefixo) == EOF) return 0;
	fscanf(fp, "%[^,],", v->prefixo);
	fscanf(fp, "%[^,],", v->data);
	fscanf(fp, "%d,", &(v->quantidadeLugares));
	fscanf(fp, "%d,", &(v->codLinha));
	fscanf(fp, "%m[^,],", &(v->modelo));
	fscanf(fp, "%m[^\n]\n", &(v->categoria));
	
	return 1;
}

int escrita (RegistroVeiculo* v, char removido, int byteProxReg, FILE* binario) {

	int tamodelo, flag = 0;
	int tamcategoria = strlen(v->categoria);
	int tam = 31 + tamcategoria;

	if (strcmp(v->modelo, "NULO") == 0) {
		flag = 1;
	} 
	else {
		tamodelo = strlen(v->modelo);
		tam += tamodelo;
	}

	fseek(binario, byteProxReg, SEEK_SET);

	fwrite(&(removido), sizeof(char), 1, binario);
	fwrite(&(tam), sizeof(int), 1, binario);
	fwrite(v->prefixo, sizeof(char), strlen(v->prefixo), binario);

	if (v->data[0] == 'N') {

		char lixo = '@';
		for (int i = 0; i < 10; i++) {
			fwrite(&lixo, sizeof(char), 1, binario); 
		}

	} else {
		fwrite(v->data, sizeof(char), strlen(v->data), binario); 
	}
	
	fwrite(&(v->quantidadeLugares), sizeof(int), 1, binario);
	
	
	if (v->codLinha == 0){

		int lixo = -1;
		fwrite(&lixo, sizeof(int), 1, binario);

	} else {
		fwrite(&(v->codLinha), sizeof(int), 1, binario); 
	}
	
	
	if (flag == 1) {

		int tam = 0;
		fwrite(&tam, sizeof(int), 1, binario);

	} else {

		fwrite(&(tamodelo), sizeof(int), 1, binario);  
		fwrite(v->modelo, sizeof(char), tamodelo, binario); 

	}
	
	
	fwrite(&(tamcategoria), sizeof(int), 1, binario);
	fwrite(v->categoria, sizeof(char), tamcategoria, binario);
	int total = tam + 5;

	return total;
}


void atualizaBinario(HeaderVeiculo* h, RegistroVeiculo* v, FILE* binario) {

	int tam;
	char removido;

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
	//printf("%ld\n", h->byteProxReg);
	
	/*
	fseek(binario, 1, SEEK_SET);
	fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
	fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
	fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
	*/
}

void imprime (RegistroVeiculo* v) {

	printf("%s\n", v->prefixo);
	printf("%s\n", v->data);
	printf("%d\n", v->quantidadeLugares);
    printf("%d\n", v->codLinha);
    printf("%s\n", v->modelo);
    printf("%s\n", v->categoria);


}

void libera (RegistroVeiculo* v) {

	if (v != NULL) {
	
		if (v->modelo != NULL) free(v->modelo);
		if (v->categoria != NULL) free(v->categoria);
		free(v);
	
	}
}

void busca (FILE* binario, char* campo, int n) {

	/*	Prefixo do veiculo: BA004
		Modelo do veiculo: NEOBUS MEGA
		Categoria do veiculo: ALIMENTADOR
		Data de entrada do veiculo na frota: 29 de maio de 2009
		Quantidade de lugares sentados disponiveis: campo com valor nulo
		--- pule uma linha em branco ---
	*/

	char removido, prefixo[6], data[11], *modelo, *categoria;
	int tam, qtdlug, tamodelo, tamcategoria, codlinha, i = 0, fdata = 0;
	fseek(binario, 175, SEEK_SET);

	while (i < n) {
		
		fread(&(removido), sizeof(char), 1, binario);
		if (removido == '0') {
			
			i++;
			printf("Registro inexistente\n");
			fread(&(tam), sizeof(int), 1, binario);
			fseek(binario, tam, SEEK_CUR);
			continue;
		}  

		fread(&(tam), sizeof(int), 1, binario);
		//printf("Tamanho do registro: %d\n", tam);

		fread(prefixo, sizeof(char), 5, binario);
		prefixo[5] = '\0';
		//printf("Prefixo do veiculo: %s\n", prefixo);

		fread(data, sizeof(char), 10, binario);
		data[10] = '\0';
		if (data[0] == '@') fdata = 1;
		//printf("Data de entrada do veiculo na frota: %s\n", data);

		fread(&(qtdlug), sizeof(int), 1, binario);
		//printf("Quantidade de lugares sentados disponiveis: %d\n", qtdlug);

		fread(&(codlinha), sizeof(int), 1, binario);
		//printf("Codigo da linha: %d\n", codlinha);

		fread(&(tamodelo), sizeof(int), 1, binario);
		if (tamodelo != 0) {
			
			modelo = (char*)malloc(sizeof(char) * tamodelo + 1); 
			fread(modelo, sizeof(char), tamodelo, binario);
			modelo[tamodelo] = '\0';
		
		}
		//printf("Modelo do veiculo: %s\n", modelo);

		fread(&(tamcategoria), sizeof(int), 1, binario);
		categoria = (char*)malloc(sizeof(char) * tamcategoria + 1); 
		fread(categoria, sizeof(char), tamcategoria, binario);
		categoria[tamcategoria] = '\0';
		//printf("Categoria do veiculo: %s\n", categoria);

		if (strcmp(campo, prefixo) == 0) {

			printf("Tamanho do registro: %d\n", tam);
			printf("Prefixo do veiculo: %s\n", prefixo);
			printf("Data de entrada do veiculo na frota: ");
			if (fdata) printf("NULO\n");
			else printf("%s\n", data);

			printf("Quantidade de lugares sentados disponiveis: %d\n", qtdlug);
			printf("Codigo da linha: %d\n", codlinha);

			printf("Modelo do veiculo: ");
			if (tamodelo) printf("%s\n", modelo);
			else printf("NULO\n");
			printf("Categoria do veiculo: %s\n", categoria);
			printf("\n");

		}

		if (tamodelo) free(modelo);
		free(categoria);

		i++;

	}

}

void funcionalidade1(FILE* fpCsv, FILE* fpBin){

}

void funcionalidade3(FILE* fpBin){

}

void funcionalidade5(FILE* fpBin, char nomeDoCampo[17], char valor[100]){

}

void funcionalidade7(FILE* fpBin, int qtdRegistros){

}