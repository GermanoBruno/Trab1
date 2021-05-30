#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "veiculo.h"

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

void readHeader(HeaderVeiculo* h, FILE* binario) {

	fread(&(h->status), sizeof(char), 1, binario);
	fread(&(h->byteProxReg), sizeof(long int), 1, binario);
	fread(&(h->nroRegistros), sizeof(int), 1, binario);
	fread(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);

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

RegistroVeiculo* create() {

	RegistroVeiculo *v = (RegistroVeiculo*)malloc(sizeof(RegistroVeiculo));
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
	strcpy(v->modelo, modelo);
	strcpy(v->categoria, categoria);

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

    int tamodelo, flagm = 0;
    int tamcategoria, flagc = 0;
    int tam = 31;

    //if (removido == '0') tam--;

    if (strcmp(v->modelo, "NULO") == 0) {
        flagm = 1;
    } 
    else {
        tamodelo = strlen(v->modelo);
        tam += tamodelo;
    }

    if (strcmp(v->categoria, "NULO") == 0) {
        flagc = 1;
    } 
    else {
        tamcategoria = strlen(v->categoria);
        tam += tamcategoria;
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



    /*if (v->codLinha == 0){

        int lixo = -1;
        fwrite(&lixo, sizeof(int), 1, binario);

    } else {
    }*/
    fwrite(&(v->codLinha), sizeof(int), 1, binario); 



    if (flagm == 1) {

        int tam = 0;
        fwrite(&tam, sizeof(int), 1, binario);

    } else {

        fwrite(&(tamodelo), sizeof(int), 1, binario);
        fwrite(v->modelo, sizeof(char), tamodelo, binario); 

    }


    if (flagc == 1) {

        int tam = 0;
        fwrite(&tam, sizeof(int), 1, binario);

    } else {

        fwrite(&(tamcategoria), sizeof(int), 1, binario);
        fwrite(v->categoria, sizeof(char), tamcategoria, binario); 

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
	//printf("%ld\n", h->byteProxReg);
	
	/*
	fseek(binario, 1, SEEK_SET);
	fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
	fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
	fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
	*/
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
/*
void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. *

	* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). *

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
*/
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

	//busca(binario, "DN020", tamveiculos);
	
	//fclose(fp);
	//fclose(binario);

	//binarioNaTela(bin);


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

