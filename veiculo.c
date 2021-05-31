#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "veiculo.h"
#include "auxiliares.h"

// Cabeçalho do arquivo de veiculos, contendo seus campos
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

// Função que cria um ponteiro para cabeçalho de veiculo e inicializa seus campos
HeaderVeiculo* criarHeader() {
	HeaderVeiculo* h = (HeaderVeiculo*)malloc(sizeof(HeaderVeiculo));
	h->status = '0';
	h->byteProxReg = 0;
	h->nroRegistros = 0;
	h->nroRegistrosRemovidos = 0;
}

// Função para debug, imprime os campos do cabeçalho h
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

// Função para ler do .csv os campos que descrevem as colunas de dados dos registros
void descreveHeader(HeaderVeiculo* h, FILE* fp) {
	fscanf(fp, "%[^,],", h->descrevePrefixo);
	fscanf(fp, "%[^,],", h->descreveData);
	fscanf(fp, "%[^,],", h->descreveLugares);
	fscanf(fp, "%[^,],", h->descreveLinha);
	fscanf(fp, "%[^,],", h->descreveModelo);
	fscanf(fp, "%[^\n]\n", h->descreveCategoria);
}

// Função que insere os valores iniciais do cabeçalho h no arquivo binario.
// Usada no início da funcionalidade 1, quando estamos começando a transferir os dados.
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

// Função que lê os campos do cabeçalho 'h' de um arquivo binario  
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

// Função que atualiza as informações do cabeçalho de um arquivo binário após a inserção dos registros
void atualizaHeader (HeaderVeiculo* h, FILE* binario) {

	fseek(binario, 0, SEEK_SET);
	h->status = '1';
	fwrite(&(h->status), sizeof(char), 1, binario);
	fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
	fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
	fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
}

// Função que libera a memória ocupada por um cabeçalho 'h'
void liberaHeader(HeaderVeiculo* h) {

    if (h != NULL) free(h);
}

// Struct do registro veículo, contendo seus campos
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

// Função que cria um ponteiro para veículo 'v', já inicializando seus campos
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

// Função auxiliar para tratar o caso de registros removidos (com prefixo começando por *)
void solveRemoved(char* prefix) {
	int tam = strlen(prefix);

	for (int i = 0; i < tam-1; i++) {
		prefix[i] = prefix[i+1];
	}
	prefix[tam-1] = '\0';
}

// Função que atribui campos fornecidos pelo usuário para um dado registro 'v'
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

// Função que lê os campos de um veículo a partir de um arquivo .csv
int leitura (FILE* fp, RegistroVeiculo* v) {

    char lixo[5];
    fscanf(fp, "%[^,],", v->prefixo);
    fscanf(fp, "%[^,],", v->data);
    
    fscanf(fp, "%[^,],", lixo);
    // Se há NULO no lugar do campo, ele recebe -1
    if (strcmp(lixo, "NULO") == 0) v->quatidadeLugares = -1;
    else {
        v->quatidadeLugares = atoi(lixo);
    }

    fscanf(fp, "%[^,],", lixo);
    // Se há NULO no lugar do campo, ele recebe -1
    if (strcmp(lixo, "NULO") == 0) v->codLinha = -1;
    else {
        v->codLinha = atoi(lixo);
    }
    
    fscanf(fp, "%m[^,],", &(v->modelo));
    fscanf(fp, "%m[^\n]\n", &(v->categoria));

    return 1;
}

// Função que escreve os campos de um dado registro 'v' num arquivo binario, a partir do byte 'byteProxReg'
int escrita (RegistroVeiculo* v, char removido, int byteProxReg, FILE* binario) {

    int flagm = 0;
    int flagc = 0;
    int tam = 31;

    // Se há NULO, marcamos isso para usar no código mais à frente
    if (strcmp(v->modelo, "NULO") == 0) {
        flagm = 1;
    } 
    // Senão, adicionamos o tamanho do campo ao tamanho do registro 
    else {
        v->tamanhoModelo = strlen(v->modelo);
        tam += v->tamanhoModelo;
    }

    // O mesmo aqui
    if (strcmp(v->categoria, "NULO") == 0) {
        flagc = 1;
    } 
    else {
        v->tamanhoCategoria = strlen(v->categoria);
        tam += v->tamanhoCategoria;
    }

    // Desloca-se o ponteiro para o byte em que começaremos a inserir
    fseek(binario, byteProxReg, SEEK_SET);

    fwrite(&(removido), sizeof(char), 1, binario);
    fwrite(&(tam), sizeof(int), 1, binario);
    fwrite(v->prefixo, sizeof(char), 5, binario);

    // A partir daqui começam as verificações de validade ou não dos campos,
    // por isso as flags marcadas anteriormente
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

    fwrite(&(v->quatidadeLugares), sizeof(int), 1, binario);

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

    // Retornamos o tamanho do registro somado aos bytes de 'removido' e 'tamanho do registro'
    // para deslocar corretamente o ponteiro na próxima inserção
    int total = tam + 5;

    return total;
}

// Função maior de escrita no arquivo binário. Usa como base a função 'escrita' anterior.
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
	
}

// Função que libera o espaço de memória ocupado por um registro 'v' e seus campos
void libera (RegistroVeiculo* v) {

	if (v != NULL) {
		if (v->modelo != NULL) free(v->modelo);
		if (v->categoria != NULL) free(v->categoria);
		free(v);
	
	}
}

// Função auxiliar que checa se chegamos ao fim do arquivo
int check(FILE* fp) {
	
	char check;
    
    if(fread(&check, sizeof(char), 1, fp) != 1) return 0;
    
    fseek(fp, -sizeof(char), SEEK_CUR);

   	return 1;
}

// Funcionalidade 1: Transferencia de dados do arquivo .csv para o binario
void func1(FILE* fp, FILE* binario) {

	HeaderVeiculo* h = criarHeader();
	
	descreveHeader(h, fp);
	setHeader(h, binario);
	
	// Enquanto não chegamos no fim do arquivo
	while (check(fp)) {
		
		RegistroVeiculo* v = create();
		leitura(fp, v);
		atualizaBinario(h, v, binario);
		libera(v);

	}

	atualizaHeader(h, binario);
	liberaHeader(h);
}

// Função auxiliar que traduz o formato da data lida (AAAA-MM-DD) para o exigido pelo trabalho 
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

// Função auxiliar para ler os campos de um registro 'v' a partir de um arquivo binário
void leituraBinario (FILE* binario, RegistroVeiculo* v, char* removido, int* tam) {

	fread(removido, sizeof(char), 1, binario);
		
		// Se o registro está removido, deslocamos o ponteiro para o próximo registro
		if (*removido == '0') {
			
			fread(tam, sizeof(int), 1, binario);
			fseek(binario, *tam, SEEK_CUR);
			return;
			
		}  

		// Daqui em diante lemos os campos do registro
		fread(tam, sizeof(int), 1, binario);

		fread(v->prefixo, sizeof(char), 5, binario);
		v->prefixo[5] = '\0';

		fread(v->data, sizeof(char), 10, binario);
		v->data[10] = '\0';

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

// Função para imprimir os campos de um dado veículo 'v'
void imprimeVeiculo(RegistroVeiculo* v) {

	printf("Prefixo do veiculo: %s\n", v->prefixo);

	printf("Modelo do veiculo: ");
	if (v->tamanhoModelo) printf("%s\n", v->modelo);
	else printf("campo com valor nulo\n");

	printf("Categoria do veiculo: ");
	if (v->tamanhoCategoria) printf("%s\n", v->categoria);
	else printf("campo com valor nulo\n");

	printf("Data de entrada do veiculo na frota: ");
	if (v->data[0] == '\0') printf("campo com valor nulo\n");
	else printData(v->data);

	if (v->quatidadeLugares != -1) printf("Quantidade de lugares sentados disponiveis: %d\n", v->quatidadeLugares);
	else printf("campo com valor nulo\n");
		
	printf("\n");
}

// Funcionalidade 3: Imprimir os registros de um arquivo binario
void func3(FILE* binario) {

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

// Função auxiliar da funcionalidade 5 que busca por registros de prefixo 'prefixo'
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
		
		if (!removido) {
			libera(v);
			continue;
		}

		if (strcmp(v->prefixo, prefixo) == 0) imprimeVeiculo(v);

		libera(v);

	}

	liberaHeader(h);
}

// Função auxiliar da funcionalidade 5 que busca por registros de data 'data'
void busca_data (FILE* binario, char* data) {

	char removido; int tam;
	HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v;

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

		if (strcmp(v->data, data) == 0) imprimeVeiculo(v);

		libera(v);

	}
	liberaHeader(h);
}

// Função auxiliar da funcionalidade 5 que busca por registros com quantidade de lugares 'qtdlug'
void busca_qtdlug (FILE* binario, int qtdlug) {

	char removido; 
	int tam;
	HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v;

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

// Função auxiliar da funcionalidade 5 que busca por registros de modelo 'modelo'
void busca_modelo (FILE* binario, char* modelo) {

	char removido; int tam;
	HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v;

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

		if (strcmp(v->modelo, modelo) == 0) imprimeVeiculo(v);

		libera(v);
	}
	liberaHeader(h);
}

// Função auxiliar da funcionalidade 5 que busca por registros de categoria 'categoria'
void busca_categoria (FILE* binario, char* categoria) {

	char removido; int tam;
	HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v;

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

		if (strcmp(v->categoria, categoria) == 0) imprimeVeiculo(v);

		libera(v);
	}
	liberaHeader(h);
}

// Funcionalidade 5: Busca por registros com campo 'nomeDoCampo' de valor 'valor'
void func5(FILE* binario, char* nomeDoCampo, char* valor) {

	if (strcmp("prefixo", nomeDoCampo) == 0) {
		busca_prefixo(binario, valor);
	}
	else if (strcmp("quantidadeLugares", nomeDoCampo) == 0) {
		
		int qtd;
		
		if (strcmp(valor, "NULO") == 0) qtd = -1;
		else qtd = atoi(valor);
		
		busca_qtdlug(binario, qtd);
	
	}
	else if (strcmp("modelo", nomeDoCampo) == 0) {
		busca_modelo(binario, valor);
	}
	else if (strcmp("data", nomeDoCampo) == 0) {
		busca_data(binario, valor);
	}
	else if (strcmp("categoria", nomeDoCampo) == 0) {
		busca_categoria(binario, valor);
	}

}

// Funcionalidade 7: Inserir 'n' registros em um arquivo binário 
int func7(FILE* binario, int n) {

	HeaderVeiculo* h = criarHeader();

	readHeaderBin(h, binario);
	
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

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
		
		setVeiculo(v, prefixo, data, lug, codl, modelo, categoria);
		atualizaBinario(h, v, binario);

		libera(v);
		
		i++;
	
	}

	atualizaHeader(h, binario);

	liberaHeader(h);
	
	return 1;
}
