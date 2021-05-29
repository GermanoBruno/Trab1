#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linhas.h"

#define DEBUG 1

struct registroLinha
{
	// Registro de tamanho variável
	char  removido; 		// 1 byte
	int   tamanhoRegistro;  // 4 bytes

	// Não pode ser nulo
	int   codLinha; 		// 4 bytes
	char  aceitaCartao;		// 1 byte
	
	int   tamanhoNome; 		// 4 bytes
	char* nomeLinha;		// variavel

	int   tamanhoCor;		// 4 bytes
	char* corLinha; 		// variavel
};
struct cabecalhoLinha
{
	// Tamanho fixo
	// Total: 82 Bytes
	char 	 status; 				//  1 byte
	long int byteProxReg;			//  8 bytes
	int 	 nroRegistros; 			//  4 bytes
	int 	 nroRegistrosRemovidos;	//  4 bytes
	char 	 descreveCodigo[16];	// 15 bytes
	char 	 descreveCartao[14];	// 13 bytes
	char 	 descreveNome[14];		// 13 bytes
	char 	 descreveCor[25]; 		// 24 bytes
};


// Cabeçalho
	HeaderLinha* criarHeaderLinha(){
		HeaderLinha* h = (HeaderLinha*)malloc(sizeof(HeaderLinha));
		h->status = '0';
		h->byteProxReg = 82;
		h->nroRegistros = 0;
		h->nroRegistrosRemovidos = 0;

		if(DEBUG){
			printf("Criou HeaderLinha\n");
			printDebugCabecalhoLinha(h);
		}


		return h;
	}

	void leituraCabecalhoLinhaCsv(HeaderLinha* h, FILE* fp){
		// HeaderLinha* h = criarHeaderLinha();

		fscanf(fp, "%[^,],", h->descreveCodigo);
		fscanf(fp, "%[^,],", h->descreveCartao);
		fscanf(fp, "%[^,],", h->descreveNome);
		fscanf(fp, "%[^\n]\n", h->descreveCor);

		if(DEBUG){
			printf("Leu cabecalhoLinha csv\n");
			printDebugCabecalhoLinha(h);
		}
	}

	HeaderLinha* leituraCabecalhoLinhaBinario(FILE* fp){
		HeaderLinha* h = (HeaderLinha*)malloc(sizeof(HeaderLinha));

		fread(&(h->status), 				sizeof(char),	  1, fp);
		fread(&(h->byteProxReg), 			sizeof(long int), 1, fp);
		fread(&(h->nroRegistros), 			sizeof(int), 	  1, fp);
		fread(&(h->nroRegistrosRemovidos), sizeof(int), 	  1, fp);
		fread(h->descreveCodigo, 		sizeof(char), 	 16, fp);
		fread(h->descreveCartao, 		sizeof(char), 	 14, fp);
		fread(h->descreveNome,			sizeof(char), 	 14, fp);
		fread(h->descreveCor, 			sizeof(char), 	 25, fp);

		if(DEBUG) {
			printf("Leu cabecalhoLinha binario\n");
			printDebugCabecalhoLinha(h);
		}

		return h;
	}

	void escritaCabecalhoLinha(HeaderLinha* h, FILE* fp){
		fwrite(&(h->status), 				 sizeof(char),     1,  fp);
		fwrite(&(h->byteProxReg), 			 sizeof(long int), 1,  fp);
		fwrite(&(h->nroRegistros),			 sizeof(int),      1,  fp);
		fwrite(&(h->nroRegistrosRemovidos), sizeof(int),      1,  fp);
		fwrite(h->descreveCodigo,		 sizeof(char),    15,  fp);
		fwrite(h->descreveCartao,		 sizeof(char),    13,  fp);
		fwrite(h->descreveNome,			 sizeof(char),    13,  fp);
		fwrite(h->descreveCor,			 sizeof(char),    24,  fp);

		if(DEBUG) {
			printf("Escreveu cabecalhoLinha\n");
			printDebugCabecalhoLinha(h);
		}
	}

	void atualizaCabecalhoLinha(HeaderLinha*h, FILE* fp){
		fseek(fp, 0, SEEK_SET);
		h->status = '1';
		fwrite(&(h->status), 				sizeof(char),      1,  fp);
		fwrite(&(h->byteProxReg), 			sizeof(long int),  1,  fp);
		fwrite(&(h->nroRegistros),			sizeof(int),       1,  fp);
		fwrite(&(h->nroRegistrosRemovidos), sizeof(int),       1,  fp);

		if(DEBUG) {
			printf("Atualizou cabecalhoLinha\n");
			printDebugCabecalhoLinha(h);
		}
	}

	void liberaCabecalhoLinha(HeaderLinha* h){
		if(h != NULL) free(h);
	}


	// DEBUG PRINT
	void printDebugCabecalhoLinha(HeaderLinha* h){
		if(h == NULL){
			printf("Nao existe header ainda\n"); 
			return;
		} 
		printf("Prints das descricoes cabecalho linha\n");
		printf("Cod: %s\n", h->descreveCodigo);
		printf("Cartao: %s\n", h->descreveCartao);
		printf("Nome: %s\n", h->descreveNome);
		printf("Cor: %s\n", h->descreveCor);
	}

	///////////////////////////////////////////////////////////////////////
// Registros
	
	RegistroLinha* criaRegistroLinha(void){
		RegistroLinha *reg = (RegistroLinha*)malloc(sizeof(RegistroLinha*));
		
		if(DEBUG){
			printf("Criou registro Linha\n");
			//printDebugRegistroLinha(reg);
		}
		return reg;
	}
	
	int leRegistroLinhaCsv(RegistroLinha* reg, FILE* fp){
		// Codigo da linha,
		// Aceita cartao,
		// Nome da linha, 
		// Cor que descreve a linha

		// ver se precisa collocar 4 pra ler o \0

		char check;
		if(fread(&check, sizeof(char), 1, fp) != 1){
			if(DEBUG) printf("Arquivo já terminado\n");
			return 0;
		}

		fseek(fp, -sizeof(char), SEEK_CUR);
		char codigoLinhaNaoTratado[3];

		fscanf(fp, "%[^,],",  codigoLinhaNaoTratado);

		if(codigoLinhaNaoTratado[0] != '*'){
			reg->codLinha = atoi(codigoLinhaNaoTratado);
			reg->removido = '1';
		}else{
			codigoLinhaNaoTratado[0] = codigoLinhaNaoTratado[1];
			codigoLinhaNaoTratado[1] = codigoLinhaNaoTratado[2];
			codigoLinhaNaoTratado[2] = '\0';
			reg->removido = '0';
			// ver se funciona esse atoi
			reg->codLinha = atoi(codigoLinhaNaoTratado);
		}

		char* aceitaCartaoNaoTratado;
		char* nomeLinhaNaoTratado;
		char* corLinhaNaoTratado;
		char  nulo[] = "NULO";

		fscanf(fp, "%m[^,],", &aceitaCartaoNaoTratado);
		fscanf(fp, "%m[^,],", &reg->nomeLinha);
		fscanf(fp, "%m[^,],", &reg->corLinha);

		// Identificação de valores nulos 
		if(strcmp(aceitaCartaoNaoTratado, nulo)){
			reg->aceitaCartao = '\0';

		}else{
			reg->aceitaCartao = aceitaCartaoNaoTratado[0];
		}		

		if(strcmp(reg->nomeLinha, nulo)){
			reg->tamanhoNome = 0;
		}else{
			//strcpy(reg->nomeLinha, nomeLinhaNaoTratado);
			reg->tamanhoNome = sizeof(*(reg->nomeLinha));
		}

		if(strcmp(reg->corLinha, nulo)){
			reg->tamanhoCor = 0;
		}else{
			//strcpy(reg->corLinha, nomeLinhaNaoTratado);
			reg->tamanhoCor = sizeof(*(reg->corLinha));
		}

		reg->tamanhoRegistro = 18 + reg->tamanhoCor + reg->tamanhoNome;
		if(DEBUG){
			printf("Nome: %s\nCor: %s\n", reg->nomeLinha, reg->corLinha);
			printf("Tamanhos: cor %d, nome %d\n", reg->tamanhoCor, reg->tamanhoNome);
			printf("Leu registro Linha\n");
			printDebugRegistroLinha(reg);
		}

		return 1;	
	}

	void liberaRegistroLinha(RegistroLinha* reg){
		if(reg != NULL){
			if(reg->nomeLinha != NULL) free(reg->nomeLinha);
			if(reg->corLinha != NULL) free(reg->corLinha);
			free(reg);
		}
		if(DEBUG){
			printf("Liberou registro Linha\n");
			//printDebugRegistroLinha(reg);
		}
	}

	void atualizaCabecalhoBinarioLinha(HeaderLinha* h, RegistroLinha* reg, FILE* fp){
		int  tam;

		if(reg->removido = '1') {
			h->nroRegistrosRemovidos++;
		}else{
			h->nroRegistros++;
		}

		tam = escreveRegistroLinha(reg, reg->removido, h->byteProxReg, fp);
		h->byteProxReg = tam + h->byteProxReg;

		fseek(fp, 1, SEEK_SET);
		fwrite(&(h->byteProxReg), 			sizeof(long int), 1, fp);
		fwrite(&(h->nroRegistros), 			sizeof(int),      1, fp);
		fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 	  1, fp);

		if(DEBUG){
			printf("Atualizou cabecalho Linha binario\n");
			printDebugCabecalhoLinha(h);
		}
	}

	void imprimeRegistroLinha(RegistroLinha* reg){
		/*
			Nome da linha: campo com valor nulo
			Cor que descreve a linha: AMARELA
			Aceita cartao: PAGAMENTO EM CARTAO SEM COBRADOR
			--- pule uma linha em branco ---
		*/

		if(reg->removido = '1'){
			printf("Registro inexistente.\n");
			return;
		}
		printf("Nome da linha: \n");
		if(reg->tamanhoNome = 0){
			printf("campo com valor nulo\n");
		}else{
			printf("%s\n", reg->nomeLinha);
		}
		printf("Cor que descreve a linha: \n");
		if(reg->tamanhoCor = 0){
			printf("campo com valor nulo\n");
		}else{
			printf("%s\n", reg->corLinha);
		}
		printf("Aceita cartao: \n");
		switch(reg->aceitaCartao){
			case 'S':
				printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
				break;
			case 'N':
				printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
				break;
			case 'F':
				printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
				break;
			case '\0':
				printf("campo com valor nulo\n");
				break;
		}
		printf("\n");

		if(DEBUG){
			printf("Imprimiu registro Linha\n");
			//printDebugRegistroLinha(reg);
		}
	}

	int escreveRegistroLinha(RegistroLinha* reg, char removido, int byteProxReg, FILE* fp){
		// Retorna o tamanho total do registro

		// Setta o ponteiro na posicao que vai escrever
		fseek(fp, byteProxReg, SEEK_SET);

		// Escrita dos dados já formatados (tamanhos fixos)
		fwrite(&(reg->removido), sizeof(char), 1, fp);
		fwrite(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
		fwrite(&(reg->codLinha), sizeof(int),  1, fp);
		fwrite(&(reg->aceitaCartao), sizeof(char), 0, fp);

		// escrita dos dados de tamanho variável
		fwrite(&(reg->tamanhoNome), sizeof(int),  1, fp);
		if(reg->tamanhoNome != 0){
			// nomeLinha
			fwrite(&(reg->nomeLinha), sizeof(char), reg->tamanhoNome, fp);
		}
		fwrite(&(reg->tamanhoCor), sizeof(int),  1, fp);
		if(reg->tamanhoCor != 0){
			// corlinha
			fwrite(&(reg->corLinha), sizeof(char), reg->tamanhoCor, fp);
		}
		if(DEBUG){
			printf("Escreveu registro Linha\n");
			printDebugRegistroLinha(reg);
		}

		return reg->tamanhoRegistro;
	}

	int leRegistroLinha(RegistroLinha* reg, FILE* fp){
		char check;
		if(fread(&check, sizeof(char), 1, fp) != 1){
			return 0;
		}
		fseek(fp, -sizeof(char), SEEK_CUR);
		// Leitura dos dados já formatados (tamanhos fixos)
		fread(&(reg->removido), sizeof(char), 1, fp);
		fread(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
		fread(&(reg->codLinha), sizeof(int),  1, fp);
		fread(&(reg->aceitaCartao), sizeof(char), 0, fp);

		// Leitura dos dados de tamanho variável
		fread(&(reg->tamanhoNome), sizeof(int),  1, fp);
		if(reg->tamanhoNome != 0){
			// nomelinha
			fread(&(reg->nomeLinha), sizeof(char), reg->tamanhoNome, fp);
		}
		fread(&(reg->tamanhoCor), sizeof(int),  1, fp);
		if(reg->tamanhoCor != 0){
			// corlinha
			fread(&(reg->corLinha), sizeof(char), reg->tamanhoCor, fp);
		}
		if(DEBUG){
			printf("Leu registro Linha\n");
			printDebugRegistroLinha(reg);
		}
		return 1;
	}

	void buscaRegistroLinha(FILE* fp){
		//RegistroLinha* reg = criaRegistroLinha();
		//HeaderLinha* h = leituraCabecalhoLinhaCsv(fp);

		// Ler cada registro, procurar o valor descrito no nome do campo
		// printar esse registro com imprimeRegistroLinha()
	}

	// DEBUG PRINT
	void printDebugRegistroLinha(RegistroLinha* reg){
		if(reg == NULL){
			printf("Não existe esse registro\n");
			return;
		}
		printf("Removido: %c\n", reg->removido);
		printf("TamReg: %d\n", reg->tamanhoRegistro);
		printf("Cod: %d\n", reg->codLinha);
		printf("Cartao: %c\n", reg->aceitaCartao);
		printf("TamNome: %d\n", reg->tamanhoNome);
		printf("Nome: %s\n", reg->nomeLinha);
		printf("TamCor: %d\n", reg->tamanhoCor);
		printf("Cor: %s\n\n", reg->corLinha);
	}

// Funcionalidades
	void funcionalidade2(FILE* fpCsv, FILE* fpBin){
		// Escrita dos registros num binario

		// Cria as structs
		HeaderLinha* h  = criarHeaderLinha();
		RegistroLinha* reg = criaRegistroLinha();
		if(DEBUG){
			printf("Func2 criou structs\n\n");
		}

		// Le e escreve o cabecalho
		leituraCabecalhoLinhaCsv(h, fpCsv);
		escritaCabecalhoLinha(h, fpBin);
		if(DEBUG){
			printf("Func2 leu e escreveu o cabecalho\n\n");
		}

		// Le e escreve cada registro
		while(leRegistroLinhaCsv(reg, fpCsv)){
			atualizaCabecalhoBinarioLinha(h, reg, fpBin);
			free(reg);			
			reg = criaRegistroLinha();
		}
		free(reg);

		if(DEBUG){
			printf("Func2 terminou\n\n");
		}
	}

	void funcionalidade4(FILE* fpBin){
	 	RegistroLinha* reg = criaRegistroLinha();
	 	if(DEBUG){
			printf("Func4 criou struct\n");
		}	
		while(leRegistroLinha(reg, fpBin)){
			imprimeRegistroLinha(reg);
			free(reg);
			reg = criaRegistroLinha();
		}
		free(reg);
		if(DEBUG){
			printf("Func4 terminou\n");
		}
	}

	void funcionalidade6(FILE* fpBin, char nomeDoCampo[17], char valor[100]){
		// Busca
	}

	void funcionalidade8(FILE* fpBin, int qtdRegistros){
		RegistroLinha* reg = criaRegistroLinha();
		for(int i = 0; i < qtdRegistros; i++){
			// LER O REGISTRO E ARMAZENAR EM reg
			// escrever no fpBin
		}
	}
