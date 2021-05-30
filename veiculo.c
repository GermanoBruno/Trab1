#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "veiculo.h"
#include "auxiliares.h"

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
	int quatidadeLugares;
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

void imprimeHeader(HeaderVeiculo* h){
	printf("Status: %c\n", h->status);
	printf("byteProxReg: %ld\n", h->byteProxReg);
	printf("nroRegistros: %d\n", h->nroRegistros);
	printf("nroRegistrosRemovidos: %d\n", h->nroRegistrosRemovidos);
	printf("descrevePrefixo: %s\n", h->descrevePrefixo);
	printf("descreveData: %s\n", h->descreveData);
	printf("descreveLinha: %s\n", h->descreveLinha);
	printf("descreveModelo: %s\n", h->descreveModelo);
	printf("descreveCategoria: %s\n", h->descreveCategoria);
}

void readHeader(HeaderVeiculo* h, FILE* binario) {
	fread(&(h->status), sizeof(char), 1, binario);
	fread(&(h->byteProxReg), sizeof(long int), 1, binario);
	fread(&(h->nroRegistros), sizeof(int), 1, binario);
	fread(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
	fread(h->descrevePrefixo, sizeof(char), 18, binario);
	fread(h->descreveData, sizeof(char), 35, binario);
	fread(h->descreveLugares, sizeof(char), 42, binario);
	fread(h->descreveLinha, sizeof(char), 26, binario);
	fread(h->descreveModelo, sizeof(char), 17, binario);
	fread(h->descreveCategoria, sizeof(char), 20, binario);
}

void descreveHeader(HeaderVeiculo* h, FILE* fp) {
	fscanf(fp, "%[^,],", h->descrevePrefixo);
	fscanf(fp, "%[^,],", h->descreveData);
	fscanf(fp, "%[^,],", h->descreveLugares);
	fscanf(fp, "%[^,],", h->descreveLinha);
	fscanf(fp, "%[^,],", h->descreveModelo);
	fscanf(fp, "%[^\n]\n", h->descreveCategoria);
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

void readHeaderBin(HeaderVeiculo* h, FILE* binario) {

    fread(&(h->status), sizeof(char), 1, binario);
    fread(&(h->byteProxReg), sizeof(long int), 1, binario);
    fread(&(h->nroRegistros), sizeof(int), 1, binario);
    fread(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
    fread(h->descrevePrefixo, sizeof(char), 18, binario); 
    fread(h->descreveData, sizeof(char), 35, binario);
    fread(h->descreveLugares, sizeof(char), 42, binario);
    fread(h->descreveLinha, sizeof(char), 26, binario);
    fread(h->descreveModelo, sizeof(char), 17, binario);
    fread(h->descreveCategoria, sizeof(char), 20, binario);
}

void atualizaHeader (HeaderVeiculo* h, FILE* binario) {

	fseek(binario, 0, SEEK_SET);
	h->status = '1';
	fwrite(&(h->status), sizeof(char), 1, binario);
	fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
	fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
	fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
}

void liberaHeader(HeaderVeiculo* h) {

    if (h != NULL) free(h);
}

RegistroVeiculo* create() {

	RegistroVeiculo *v = (RegistroVeiculo*)malloc(sizeof(RegistroVeiculo));
	v->modelo = NULL;
	v->categoria = NULL;
	v->quatidadeLugares = 0;
	v->codLinha = 0;
	v->removido = '0';
	v->tamanhoRegistro = 0;
	v->tamanhoModelo = 0;
	v->tamanhoCategoria = 0;
	return v;
}

void solveRemoved(char* prefix) {
	int tam = strlen(prefix);

	for (int i = 0; i < tam-1; i++) {
		prefix[i] = prefix[i+1];
	}
	prefix[tam-1] = '\0';
}

void setVeiculo(RegistroVeiculo* v, char* prefixo, char* data, int lug, int codl, char* modelo, char* categoria) {

    strcpy(v->prefixo, prefixo);
    strcpy(v->data, data);
    v->quatidadeLugares = lug;
    v->codLinha = codl;

    v->modelo = (char*)malloc(sizeof(char)*strlen(modelo)+1);
    strcpy(v->modelo, modelo);
    v->modelo[strlen(modelo)] = '\0';

    v->categoria = (char*)malloc(sizeof(char)*strlen(categoria)+1);
    strcpy(v->categoria, categoria);
    v->categoria[strlen(categoria)] = '\0';

}
int leitura (FILE* fp, RegistroVeiculo* v) {
    //if (fscanf(fp, "%[^,],", v->prefixo) == EOF) return 0;
    char lixo[5];
    fscanf(fp, "%[^,],", v->prefixo);
    fscanf(fp, "%[^,],", v->data);
    fscanf(fp, "%d,", &(v->quatidadeLugares));
    fscanf(fp, "%[^,],", lixo);
    if (strcmp(lixo, "NULO") == 0) v->codLinha = -1;
    else {
        v->codLinha = atoi(lixo);
    }
    //fscanf(fp, "%d,", &(v->codLinha));
    fscanf(fp, "%m[^,],", &(v->modelo));
    fscanf(fp, "%m[^\n]\n", &(v->categoria));

    return 1;
}

int escrita (RegistroVeiculo* v, char removido, int byteProxReg, FILE* binario) {

	// NEM tamodelo NEM tamcateogoria SAO NECESSARIOS!!! USAR OS PROPRIOS CAMPOS DO VEICULO
    int flagm = 0;
    int flagc = 0;
    int tam = 31;

    //if (removido == '0') tam--;

    if (strcmp(v->modelo, "NULO") == 0) {
        flagm = 1;
    } 
    else {
        v->tamanhoModelo = strlen(v->modelo);
        tam += v->tamanhoModelo;
    }

    if (strcmp(v->categoria, "NULO") == 0) {
        flagc = 1;
    } 
    else {
        v->tamanhoCategoria = strlen(v->categoria);
        tam += v->tamanhoCategoria;
    }

    fseek(binario, byteProxReg, SEEK_SET);

    fwrite(&(removido), sizeof(char), 1, binario);
    fwrite(&(tam), sizeof(int), 1, binario);
    fwrite(v->prefixo, sizeof(char), 5, binario);

    if (v->data[0] == 'N') {

        char lixo = '\0';
        fwrite(&lixo, sizeof(char), 1, binario);
        lixo = '@';
        for (int i = 0; i < 9; i++) {
            fwrite(&lixo, sizeof(char), 1, binario); 
        }

    } else {
        fwrite(v->data, sizeof(char), strlen(v->data), binario); 
    }


    if (v->quatidadeLugares == 0) {

        int lixo = -1;
        fwrite(&(lixo), sizeof(int), 1, binario);

    } else {
        fwrite(&(v->quatidadeLugares), sizeof(int), 1, binario);
    }

    fwrite(&(v->codLinha), sizeof(int), 1, binario); 

    if (flagm == 1) {
        int tam = 0;
        fwrite(&tam, sizeof(int), 1, binario);
    } else {

        fwrite(&(v->tamanhoModelo), sizeof(int), 1, binario);
        fwrite(v->modelo, sizeof(char), v->tamanhoModelo, binario); 
    }
    if (flagc == 1) {
        int tam = 0;
        fwrite(&tam, sizeof(int), 1, binario);
    } else {
        fwrite(&(v->tamanhoCategoria), sizeof(int), 1, binario);
        fwrite(v->categoria, sizeof(char), v->tamanhoCategoria, binario); 
    }

    int total = tam + 5;
    return total;
}

void atualizaBinario(HeaderVeiculo* h, RegistroVeiculo* v, FILE* binario) {

	int tam;
	char removido;

	if (v->prefixo[0] == '*') {
		solveRemoved(v->prefixo);
		h->nroRegistrosRemovidos++;
		removido = '0'; 
	}
	else  {
		h->nroRegistros++;
		removido = '1';
	}

	tam = escrita(v, removido, h->byteProxReg, binario);

	
	h->byteProxReg = h->byteProxReg + tam;
	//printf("bpr = %ld\n", h->byteProxReg);
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

int check(FILE* fp) {
	
	char check;
    
    if(fread(&check, sizeof(char), 1, fp) != 1) return 0;
    
    fseek(fp, -sizeof(char), SEEK_CUR);

   	return 1;
}

void func1(FILE* fp, FILE* binario) {
	//FILE *fp, *binario;
	HeaderVeiculo* h = criarHeader();
	
	//fp = fopen(csv, "r");
	descreveHeader(h, fp);
	//binario = fopen(bin, "w+b");
	setHeader(h, binario);
	
	//int i = 0;
	while (check(fp)) { 	// Fazer função pra ler até o fim do arquivo depois
		
		RegistroVeiculo* v = create();
		leitura(fp, v);
		atualizaBinario(h, v, binario);
		libera(v);
		//i++;

	}
	atualizaHeader(h, binario);
	liberaHeader(h);
}

void printData(char* data) {


	char ano[5], mes[3], dia[3];

	for (int i = 0; i < 4; i++) {
		ano[i] = data[i];
	}
	ano[4] = '\0';

	for (int i = 0, j = 5; j < 7; i++, j++) {
		mes[i] = data[j];
	}
	mes[2] = '\0';

	for (int i = 0, j = 8; j < 10; i++, j++) {
		dia[i] = data[j];
	}
	dia[2] = '\0';

	int m = atoi(mes);

	switch (m) {

		case 1:
			printf("%s de janeiro de %s\n", dia, ano);
			break;
		case 2:
			printf("%s de fevereiro de %s\n", dia, ano);
			break;
		case 3:
			printf("%s de março de %s\n", dia, ano);
			break;
		case 4:
			printf("%s de abril de %s\n", dia, ano);
			break;
		case 5:
			printf("%s de maio de %s\n", dia, ano);
			break;
		case 6:
			printf("%s de junho de %s\n", dia, ano);
			break;
		case 7:
			printf("%s de julho de %s\n", dia, ano);
			break;
		case 8:
			printf("%s de agosto de %s\n", dia, ano);
			break;
		case 9:
			printf("%s de setembro de %s\n", dia, ano);
			break;
		case 10:
			printf("%s de outubro de %s\n", dia, ano);
			break;
		case 11:
			printf("%s de novembro de %s\n", dia, ano);
			break;
		case 12:
			printf("%s de dezembro de %s\n", dia, ano); 
			break;

	}
}

void func3(FILE* binario) {

	//FILE *binario;
	//binario = fopen(bin, "rb");

	char removido, prefixo[6], data[11], *modelo, *categoria;
	int tam, qtdlug, tamodelo, tamcategoria, codlinha;// fdata = 0;
	int flag = 0;
	HeaderVeiculo* h = criarHeader();
	//printf("É aqui??\n");
	readHeaderBin(h, binario);
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	//printf("Ou aqui??\n");

	if(h->nroRegistros == 0){
		printf("Registro inexistente.\n");
		return;
	}

	while (check(binario)) {
		fread(&(removido), sizeof(char), 1, binario);
		
		if (removido == '0') {
			fread(&(tam), sizeof(int), 1, binario);
			fseek(binario, tam, SEEK_CUR);
			continue;
			
		}  

		fread(&(tam), sizeof(int), 1, binario);
		flag = 1;

		fread(prefixo, sizeof(char), 5, binario);
		prefixo[5] = '\0';

		fread(data, sizeof(char), 10, binario);
		data[10] = '\0';
		//if (data[1] == '@') fdata = 1;

		fread(&(qtdlug), sizeof(int), 1, binario);

		fread(&(codlinha), sizeof(int), 1, binario);

		fread(&(tamodelo), sizeof(int), 1, binario);
		if (tamodelo != 0) {
			
			modelo = (char*)malloc(sizeof(char) * tamodelo + 1); 
			fread(modelo, sizeof(char), tamodelo, binario);
			modelo[tamodelo] = '\0';
		
		}

		fread(&(tamcategoria), sizeof(int), 1, binario);
		if (tamcategoria != 0) {
			
			categoria = (char*)malloc(sizeof(char) * tamcategoria + 1); 
			fread(categoria, sizeof(char), tamcategoria, binario);
			categoria[tamcategoria] = '\0';
		
		}
		

		printf("Prefixo do veiculo: %s\n", prefixo);

		printf("Modelo do veiculo: ");
		if (tamodelo) printf("%s\n", modelo);
		else printf("campo com valor nulo\n");

		if (tamcategoria) printf("Categoria do veiculo: %s\n", categoria);
		else printf("campo com valor nulo\n");

		printf("Data de entrada do veiculo na frota: ");
		if (data[1] == '@') printf("campo com valor nulo\n");
		else printData(data);

		if (qtdlug != -1) printf("Quantidade de lugares sentados disponiveis: %d\n", qtdlug);
		else printf("campo com valor nulo\n");
		
		printf("\n");

		if (tamodelo) free(modelo);
		if (tamcategoria) free(categoria);

	}
}

void leituraBinario (FILE* binario, RegistroVeiculo* v, char* removido, int* tam) {

	fread(removido, sizeof(char), 1, binario);
		
		if (*removido == '0') {
			
			fread(tam, sizeof(int), 1, binario);
			fseek(binario, *tam, SEEK_CUR);
			return;
			
		}  

		fread(tam, sizeof(int), 1, binario);

		fread(v->prefixo, sizeof(char), 5, binario);
		v->prefixo[5] = '\0';

		fread(v->data, sizeof(char), 10, binario);
		v->data[10] = '\0';
		//if (data[1] == '@') fdata = 1;

		fread(&(v->quatidadeLugares), sizeof(int), 1, binario);

		fread(&(v->codLinha), sizeof(int), 1, binario);

		fread(&(v->tamanhoModelo), sizeof(int), 1, binario);
		if (v->tamanhoModelo != 0) {
			
			v->modelo = (char*)malloc(sizeof(char) * v->tamanhoModelo + 1); 
			fread(v->modelo, sizeof(char), v->tamanhoModelo, binario);
			v->modelo[v->tamanhoModelo] = '\0';
		
		}

		fread(&(v->tamanhoCategoria), sizeof(int), 1, binario);
		if (v->tamanhoCategoria != 0) {
			
			v->categoria = (char*)malloc(sizeof(char) * v->tamanhoCategoria + 1); 
			fread(v->categoria, sizeof(char), v->tamanhoCategoria, binario);
			v->categoria[v->tamanhoCategoria] = '\0';
		
		}
}

void imprimeVeiculo(RegistroVeiculo* v) {

	printf("Prefixo do veiculo: %s\n", v->prefixo);

	printf("Modelo do veiculo: ");
	if (v->tamanhoModelo) printf("%s\n", v->modelo);
	else printf("campo com valor nulo\n");

	if (v->tamanhoCategoria) printf("Categoria do veiculo: %s\n", v->categoria);
	else printf("campo com valor nulo\n");

	printf("Data de entrada do veiculo na frota: ");
	if (v->data[1] == '@') printf("campo com valor nulo\n");
	else printData(v->data);

	if (v->quatidadeLugares != -1) printf("Quantidade de lugares sentados disponiveis: %d\n", v->quatidadeLugares);
	else printf("campo com valor nulo\n");
		
	printf("\n");
}

void busca_prefixo (FILE* binario, char* prefixo) {

	char removido; int tam;
	HeaderVeiculo* h = criarHeader();

	readHeaderBin(h, binario);
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	
	if(h->nroRegistros == 0){
		printf("Registro inexistente.\n");
		return;
	}

	RegistroVeiculo *v;
	while (check(binario)){
		v = create();
		leituraBinario(binario, v, &removido, &tam);
		if (!removido) continue;

		if (strcmp(v->prefixo, prefixo) == 0) imprimeVeiculo(v);

		libera(v);
	}
	liberaHeader(h);
}

void busca_data (FILE* binario, char* data) {

	char removido; int tam;
	HeaderVeiculo* h = criarHeader(); 
	RegistroVeiculo* v = create();

	readHeaderBin(h, binario);
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if(h->nroRegistros == 0){
		printf("Registro inexistente.\n");
		return;
	}

	while (check(binario)) {
		
		v = create();
		leituraBinario(binario, v, &removido, &tam);
		if (!removido) continue;

		if (strcmp(v->data, data) == 0) imprimeVeiculo(v);

		libera(v);
	}
	liberaHeader(h);
}

void busca_qtdlug (FILE* binario, int qtdlug) {

	char removido; 
	int tam;
	HeaderVeiculo* h = criarHeader();
	RegistroVeiculo* v;

	readHeaderBin(h, binario);
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if(h->nroRegistros == 0){
		printf("Registro inexistente.\n");
		return;
	}
	
	while (check(binario)) {
		
		v = create();
		leituraBinario(binario, v, &removido, &tam);
		if (!removido) {
			libera(v);
			continue;
		}


		if (v->quatidadeLugares == qtdlug) imprimeVeiculo(v);

		libera(v);
	}
	liberaHeader(h);
}

void busca_modelo (FILE* binario, char* modelo) {

	char removido; int tam;
	HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v = create();

	readHeaderBin(h, binario);
	
	if(h->nroRegistros == 0){
		printf("Registro inexistente.\n");
		return;
	}

	while (check(binario)) {
		
		v = create();
		leituraBinario(binario, v, &removido, &tam);
		if (!removido) continue;

		if (strcmp(v->modelo, modelo) == 0) imprimeVeiculo(v);

		libera(v);
	}
	liberaHeader(h);
}

void busca_categoria (FILE* binario, char* categoria) {

	char removido; int tam;
	HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v = create();

	readHeaderBin(h, binario);
	
	if(h->nroRegistros == 0){
		printf("Registro inexistente.\n");
		return;
	}

	while (check(binario)) {
		
		v = create();
		
		leituraBinario(binario, v, &removido, &tam);
		if (!removido) continue;

		if (strcmp(v->categoria, categoria) == 0) imprimeVeiculo(v);

		libera(v);
	}
	liberaHeader(h);
}

void func5(FILE* binario, char* nomeDoCampo, char* valor) {

	if (strcmp("prefixo", nomeDoCampo) == 0) {
		//Funcao fornecida pra ler o campo
		busca_prefixo(binario, valor);
	}else if (strcmp("quantidadeLugares", nomeDoCampo) == 0) {
		int qtd = atoi(valor);
		busca_qtdlug(binario, qtd);
	}else if (strcmp("modelo", nomeDoCampo) == 0) {
		busca_modelo(binario, valor);
	}else if (strcmp("data", nomeDoCampo) == 0) {
		busca_data(binario, valor);
	}else if (strcmp("categoria", nomeDoCampo) == 0) {
		busca_categoria(binario, valor);
	}
}

int func7(FILE* binario, int n) {
	//FILE *fp, binario;
	HeaderVeiculo* h = criarHeader();

	readHeader(h, binario);
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	//imprimeHeader(h);
	//printf("Bpr = %ld\n", h->byteProxReg);

	char prefixo[7];
	char data[12];
	int lug = 0;
	char lugNaoTratado[4];
	char codNaoTratado[6];
	int codl = 0;
	char modelo[100];
	char categoria[100];
	int i = 0;

	while (i < n) { 
		RegistroVeiculo* v = create();
		
		//Ler direito
		scan_quote_string(prefixo);
		scan_quote_string(data);
		if(strcmp(data, "") == 0){
			strcpy(data, "NULO");
		}
		scanf("%s %s", lugNaoTratado, codNaoTratado);
		if((lugNaoTratado[0] == 'N') || (lugNaoTratado[0] == 'n')){
			lug = -1;
		}else{
			lug = atoi(lugNaoTratado);
		}
		if((codNaoTratado[0] == 'N') || (codNaoTratado[0] == 'n')){
			codl = -1;
		}else{
			codl = atoi(codNaoTratado);
		}
		scan_quote_string(modelo);
		if(strcmp(modelo, "") == 0){
			strcpy(modelo, "NULO");
		}
		scan_quote_string(categoria);
		if(strcmp(categoria, "") == 0){
			strcpy(categoria, "NULO");
		}
		/*printf("pref: %s\n", prefixo);
		printf("data: %s\n", data);
		printf("lug: %d\n", lug);
		printf("cod: %d\n", codl);
		printf("mod: %s\n", modelo);
		printf("cat: %s\n", categoria);*/
		
		setVeiculo(v, prefixo, data, lug, codl, modelo, categoria);
		atualizaBinario(h, v, binario);
		//imprimeVeiculo(v);
		libera(v);
		i++;
	}
	atualizaHeader(h, binario);

	liberaHeader(h);
	return 1;
}
