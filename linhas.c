#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linhas.h"
#include "auxiliares.h"


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
		// Inicializa o cabeçalho, settando os primeiros valores
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
		// Lê o cabeçalho no csv e guarda na Struct HeaderLinha

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
		// Le o cabecalho em um arquivo binario e salva
		// na struct HeaderLinha

		HeaderLinha* h = (HeaderLinha*)malloc(sizeof(HeaderLinha));

		fread(&(h->status), 				sizeof(char),	  1, fp);
		fread(&(h->byteProxReg), 			sizeof(long int), 1, fp);
		fread(&(h->nroRegistros), 			sizeof(int), 	  1, fp);
		fread(&(h->nroRegistrosRemovidos), 	sizeof(int), 	  1, fp);
		fread(h->descreveCodigo, 			sizeof(char), 	 15, fp);
		h->descreveCodigo[15] = '\0';
		fread(h->descreveCartao, 			sizeof(char), 	 13, fp);
		h->descreveCartao[13] = '\0';
		fread(h->descreveNome,				sizeof(char), 	 13, fp);
		h->descreveNome[13] = '\0';
		fread(h->descreveCor, 				sizeof(char), 	 24, fp);
		h->descreveCor[24] = '\0';

		if(DEBUG) {
			printf("Leu cabecalhoLinha binario\n");
			printDebugCabecalhoLinha(h);
		}
		return h;
	}

	void escritaCabecalhoLinha(HeaderLinha* h, FILE* fp){
		// Escreve o cabeçalho no começo do arquivo binario
		fwrite(&(h->status), 				 sizeof(char),     1,  fp);
		fwrite(&(h->byteProxReg), 			 sizeof(long int), 1,  fp);
		fwrite(&(h->nroRegistros),			 sizeof(int),      1,  fp);
		fwrite(&(h->nroRegistrosRemovidos),  sizeof(int),      1,  fp);
		fwrite(h->descreveCodigo,		 	 sizeof(char),    15,  fp);
		fwrite(h->descreveCartao,			 sizeof(char),    13,  fp);
		fwrite(h->descreveNome,				 sizeof(char),    13,  fp);
		fwrite(h->descreveCor,			 	 sizeof(char),    24,  fp);

		if(DEBUG) {
			printf("Escreveu cabecalhoLinha\n");
			printDebugCabecalhoLinha(h);
		}
	}

	void atualizaHeaderLinha (HeaderLinha* h, FILE* binario) {
		// Atualiza o cabecalho de um arquivo binario com os valores
		// presentes na struct HeaderLinha
        fseek(binario, 0, SEEK_SET);
        h->status = '1';
        fwrite(&(h->status), sizeof(char), 1, binario);
        fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
        fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
        fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
    }

	void liberaCabecalhoLinha(HeaderLinha* h){
		// Libera a struct HeaderLinha na memoria
		if(h != NULL) free(h);
	}

	// DEBUG PRINT
	void printDebugCabecalhoLinha(HeaderLinha* h){
		if(h == NULL){
			printf("Nao existe header ainda\n"); 
			return;
		} 
		printf("Prints das descricoes cabecalho linha\n");
		printf("Cod: %s \n", h->descreveCodigo);
		printf("Cartao: %s \n", h->descreveCartao);
		printf("Nome: %s \n", h->descreveNome);
		printf("Cor: %s \n", h->descreveCor);
	}

	///////////////////////////////////////////////////////////////////////
// Registros
	
	RegistroLinha* criaRegistroLinha(void){
		// Cria a struct registrolinha e aloca espaco na memoria pra ela
		// Inicializa com valores nulos
		RegistroLinha *reg = (RegistroLinha*)malloc(sizeof(RegistroLinha));
		
		reg->removido = '0';
		reg->tamanhoRegistro = 0;
		reg->codLinha = 0;
		reg->aceitaCartao = '0';
		reg->tamanhoNome = 0;
		reg->nomeLinha = NULL;
		reg->tamanhoCor = 0;
		reg->corLinha = NULL;

		if(DEBUG){
			printf("Criou registro Linha\n");
			//printDebugRegistroLinha(reg);
		}
		return reg;
	}
	
	int leRegistroLinhaCsv(RegistroLinha* reg, FILE* fp){
		// Le uma linha de registro no csv
		// Retorna o tamanho do registro

		char codigoLinhaNaoTratado[4];

		fscanf(fp, "%[^,],",  codigoLinhaNaoTratado);

		// Tratamento do codigo linha, verificando se esta removido
		if(codigoLinhaNaoTratado[0] != '*'){
			reg->removido = '1';
			reg->codLinha = atoi(codigoLinhaNaoTratado);
		}else{
			codigoLinhaNaoTratado[0] = codigoLinhaNaoTratado[1];
			codigoLinhaNaoTratado[1] = codigoLinhaNaoTratado[2];
			codigoLinhaNaoTratado[2] = '\0';
			reg->removido = '0';
			reg->codLinha = atoi(codigoLinhaNaoTratado);
		}

		char* aceitaCartaoNaoTratado;
		char* nomeLinhaNaoTratado;
		char* corLinhaNaoTratado;
		char  nulo[] = "NULO";

		// Identificação de valores nulos 
		// e passa os valores lidos para a struct registroLinha
		fscanf(fp, "%m[^,],", &aceitaCartaoNaoTratado);
		if(strcmp(aceitaCartaoNaoTratado, nulo) == 0){
			reg->aceitaCartao = '\0';

		}else{
			reg->aceitaCartao = aceitaCartaoNaoTratado[0];
		}		

		fscanf(fp, "%m[^,],", &nomeLinhaNaoTratado);
		if(strcmp(nomeLinhaNaoTratado, nulo) == 0){
			reg->tamanhoNome = 0;
		}else{
			reg->tamanhoNome = strlen(nomeLinhaNaoTratado);
			reg->nomeLinha = (char*)malloc(1 + sizeof(char) * (reg->tamanhoNome));
			
			strcpy(reg->nomeLinha, nomeLinhaNaoTratado);
		}

		fscanf(fp, "%m[^\n]\n", &corLinhaNaoTratado);
		if(strcmp(corLinhaNaoTratado, nulo) == 0){
			reg->tamanhoCor = 0;
		}else{
			reg->tamanhoCor = strlen(corLinhaNaoTratado);
			reg->corLinha = (char*)malloc(1 + sizeof(char) * (reg->tamanhoCor));
			strcpy(reg->corLinha, corLinhaNaoTratado);
		}

		// Setta o tamanho total do registro como o tamanho fixo e 
		// soma os tamanhos variáveis
		reg->tamanhoRegistro = 13 + reg->tamanhoCor + reg->tamanhoNome;
		free(aceitaCartaoNaoTratado);
		free(nomeLinhaNaoTratado);
		free(corLinhaNaoTratado);
		if(DEBUG){
			printf("Nome: %s\nCor: %s\n", reg->nomeLinha, reg->corLinha);
			printf("Tamanhos: cor %d, nome %d\n", reg->tamanhoCor, reg->tamanhoNome);
			printf("Leu registro Linha\n");
			printDebugRegistroLinha(reg);
		}
		return 1;	
	}

	void liberaRegistroLinha(RegistroLinha* reg){
		// Libera a struct registrolinha na memoria

		if(reg != NULL){
			//printf("vai liberar nome\n");
			if(reg->nomeLinha != NULL) free(reg->nomeLinha);
			//printf("vai liberar cor\n");
			if(reg->corLinha != NULL) free(reg->corLinha);
			//printf("vai liberar reg\n");
			free(reg);
		}
		if(DEBUG){
			printf("Liberou registro Linha\n");
			//printDebugRegistroLinha(reg);
		}
	}

	void atualizaCabecalhoBinarioLinha(HeaderLinha* h, RegistroLinha* reg, FILE* fp){
        // Atualiza o cabecalho e escreve um registro na memoria

        int tam = 5;

        if(reg->removido == '0') {
            h->nroRegistrosRemovidos++;
        }else{
            h->nroRegistros++;
        }

        tam += escreveRegistroLinha(reg, h->byteProxReg, fp);
        h->byteProxReg = tam + h->byteProxReg;
        h->status = '1';

        if(DEBUG){
            printf("Atualizou cabecalho Linha binario\n");
            printDebugCabecalhoLinha(h);
        }
    }
	

	void imprimeRegistroLinha(RegistroLinha* reg){
		// Imprime o registroLinha de acordo com a especificacao dada


		if(reg->removido == '0'){
			return;
		}
		printf("Codigo da linha: %d\n", reg->codLinha);
		printf("Nome da linha: ");
		// Tratamento de nulos
		if(reg->tamanhoNome == 0){
			printf("campo com valor nulo\n");
		}else{
			printf("%s\n", reg->nomeLinha);
		}
		printf("Cor que descreve a linha: ");
		if(reg->tamanhoCor == 0){
			printf("campo com valor nulo\n");
		}else{
			printf("%s\n", reg->corLinha);
		}
		// Tratamento do aceitaCartao
		printf("Aceita cartao: ");
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

	int escreveRegistroLinha(RegistroLinha* reg, int byteProxReg, FILE* fp){
		// Escreve um registro lido no arquivo binario
		// Retorna o tamanho total do registro

		// Setta o ponteiro na posicao que vai escrever
		fseek(fp, byteProxReg, SEEK_SET);

		// Escrita dos dados já formatados (tamanhos fixos)
		fwrite(&(reg->removido), 		sizeof(char), 1, fp);
		fwrite(&(reg->tamanhoRegistro), sizeof(int),  1, fp);
		fwrite(&(reg->codLinha), 		sizeof(int),  1, fp);
		fwrite(&(reg->aceitaCartao), 	sizeof(char), 1, fp);

		// escrita dos dados de tamanho variável
		fwrite(&(reg->tamanhoNome), sizeof(int),  1, fp);
		if(reg->tamanhoNome != 0){
			fwrite(reg->nomeLinha, sizeof(char), reg->tamanhoNome, fp);
		}
		fwrite(&(reg->tamanhoCor), sizeof(int),  1, fp);
		if(reg->tamanhoCor != 0){
			fwrite(reg->corLinha, sizeof(char), reg->tamanhoCor, fp);
		}
		if(DEBUG){
			printf("Escreveu registro Linha\n");
			printDebugRegistroLinha(reg);
		}

		return reg->tamanhoRegistro;
	}

	int leRegistroLinha(RegistroLinha* reg, FILE* fp){
		// Le um registro formatado no arquivo binario e salva
		// na struct registroLinha


		// Leitura dos dados já formatados (tamanhos fixos)
		fread(&(reg->removido), 		sizeof(char), 1, fp);
		fread(&(reg->tamanhoRegistro), 	sizeof(int),  1, fp);
		fread(&(reg->codLinha), 		sizeof(int),  1, fp);
		fread(&(reg->aceitaCartao), 	sizeof(char), 1, fp);

		// Leitura dos dados de tamanho variável
		fread(&(reg->tamanhoNome), 		sizeof(int),  1, fp);
		if(reg->tamanhoNome != 0){
			reg->nomeLinha = (char*)malloc((sizeof(char) * (reg->tamanhoNome+1)));
			fread(reg->nomeLinha, 	sizeof(char), reg->tamanhoNome, fp);
			reg->nomeLinha[reg->tamanhoNome] = '\0';
		}
		fread(&(reg->tamanhoCor), 		sizeof(int),  1, fp);
		if(reg->tamanhoCor != 0){
			reg->corLinha = (char*)malloc((sizeof(char)* (reg->tamanhoCor+1)));
			fread(reg->corLinha, sizeof(char), reg->tamanhoCor, fp);
			reg->corLinha[reg->tamanhoCor] = '\0';
		}
		if(DEBUG){
			printf("Leu registro Linha\n");
			printDebugRegistroLinha(reg);
		}
		return 1;
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
		printf("Nome: %s\n",  reg->nomeLinha);
		printf("TamCor: %d\n", reg->tamanhoCor);
		printf("Cor: %s\n\n", reg->corLinha);
	}

	int checkFile(FILE* fp) {
		// Funcao para verificar se o arquivo terminou

		char check;
	    if(fread(&check, sizeof(char), 1, fp) != 1) return 0;
	    fseek(fp, -sizeof(char), SEEK_CUR);
   		return 1;
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
		while(checkFile(fpCsv)){
			leRegistroLinhaCsv(reg, fpCsv);
			atualizaCabecalhoBinarioLinha(h, reg, fpBin);
			liberaRegistroLinha(reg);			
			reg = criaRegistroLinha();
		}
		atualizaHeaderLinha(h, fpBin);
		liberaCabecalhoLinha(h);
		liberaRegistroLinha(reg);

		if(DEBUG){
			printf("Func2 terminou\n\n");
		}
	}

	void funcionalidade4(FILE* fpBin){
		// Le o arquivo binario e imprime todos os registros não removidos

	 	RegistroLinha* reg = criaRegistroLinha();
	 	HeaderLinha* h = leituraCabecalhoLinhaBinario(fpBin);
	 	if(h->status == '0'){
	 		// verificacao de status
			printf("Falha no processamento do arquivo.\n");
		}
	 	if(DEBUG){
			printf("Func4 criou struct\n");
		}	
		if(h->nroRegistros == 0){
			// Se não houver registros, não tem o que imprimir
			printf("Registro inexistente.\n");
			return;
		}
		while(checkFile(fpBin)){
			// Se o arquivo não terminou, le os registros e imprime
			leRegistroLinha(reg, fpBin);
			imprimeRegistroLinha(reg);
			liberaRegistroLinha(reg);
			reg = criaRegistroLinha();

		}
		atualizaHeaderLinha(h, fpBin);
		liberaCabecalhoLinha(h);
		liberaRegistroLinha(reg);
		if(DEBUG){
			printf("Func4 terminou\n");
		}
	}

	void buscaCartao(FILE* fp, char* valor){
		// Imprime os registros com aceitaCartao igual ao valor passado
		HeaderLinha* h = leituraCabecalhoLinhaBinario(fp);
		if(h->status == '0'){
			// verificacao de status
			printf("Falha no processamento do arquivo.\n");
		}
		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}

		RegistroLinha* reg;
		while (checkFile(fp)){
			// Verificando se o arquivo nao terminou
			reg = criaRegistroLinha();
			// le o registro e verifica se não está removido
			leRegistroLinha(reg, fp);
			if (!reg->removido) continue;
			//Se o valor é igual, imprime o registro
			if (reg->aceitaCartao == *valor) imprimeRegistroLinha(reg);

			liberaRegistroLinha(reg);
		}
		liberaCabecalhoLinha(h);
	}
	void buscaNome(FILE* fp, char* valor){
		// Imprime os registros com aceitaCartao igual ao valor passado
		HeaderLinha* h = leituraCabecalhoLinhaBinario(fp);
		if(h->status == '0'){
			// verificacao de status
			printf("Falha no processamento do arquivo.\n");
		}
		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}

		RegistroLinha* reg;
		while (checkFile(fp)){
			// Verificando se o arquivo nao terminou
			reg = criaRegistroLinha();
			// le o registro e verifica se não está removido
			leRegistroLinha(reg, fp);
			if (!reg->removido) continue;
			//Se o valor é igual, imprime o registro
			if (strcmp(reg->nomeLinha, valor) == 0) imprimeRegistroLinha(reg);

			liberaRegistroLinha(reg);
		}
		liberaCabecalhoLinha(h);
	}
	void buscaCodigo(FILE* fp, char* valor){
		// Imprime os registros com aceitaCartao igual ao valor passado

		HeaderLinha* h = leituraCabecalhoLinhaBinario(fp);
		if(h->status == '0'){
			// verificacao de status
			printf("Falha no processamento do arquivo.\n");
		}
		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}

		RegistroLinha* reg;
		while (checkFile(fp)){
			// Verificando se o arquivo nao terminou
			reg = criaRegistroLinha();
			// le o registro e verifica se não está removido
			leRegistroLinha(reg, fp);
			if (!reg->removido) continue;
			//Se o valor é igual, imprime o registro
			int cod = atoi(valor);
			if (reg->codLinha == cod) imprimeRegistroLinha(reg);

			liberaRegistroLinha(reg);
		}
		liberaCabecalhoLinha(h);
	}
	void buscaCor(FILE* fp, char* valor){
		// Imprime os registros com aceitaCartao igual ao valor passado

		HeaderLinha* h = leituraCabecalhoLinhaBinario(fp);
		if(h->status == '0'){
			// verificacao de status
			printf("Falha no processamento do arquivo.\n");
		}
		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}
		RegistroLinha* reg;

		while (checkFile(fp)){
			// Verificando se o arquivo nao terminou
			reg = criaRegistroLinha();
			// le o registro e verifica se não está removido
			leRegistroLinha(reg, fp);
			if (!reg->removido) continue;
			//Se o valor é igual, imprime o registro

			if (strcmp(reg->corLinha, valor) == 0) imprimeRegistroLinha(reg);

			liberaRegistroLinha(reg);
		}
		liberaCabecalhoLinha(h);
	}

	void funcionalidade6(FILE* fpBin, char* nomeDoCampo, char* valor){
		// Verifica qual o nome do campo buscado e chama a funcao 
		// correspondente à essa busca

		if(strcmp("aceitaCartao", nomeDoCampo) == 0){
			buscaCartao(fpBin, valor);
		}else if(strcmp("nomeLinha", nomeDoCampo) == 0){
			buscaNome(fpBin, valor);
		}else if(strcmp("corLinha", nomeDoCampo) == 0){
			buscaCor(fpBin, valor);
		}else if(strcmp("codLinha", nomeDoCampo) == 0){
			buscaCodigo(fpBin, valor);
		}
	}

	void setRegistroLinha(RegistroLinha* reg, int cod, char cartao, char* nome, char* cor){
		// Atualiza o registroLinha com dados passados pela funcao

		reg->codLinha = cod;
		reg->aceitaCartao = cartao;
		
		reg->tamanhoNome = strlen(nome);
		reg->nomeLinha = (char*)malloc(sizeof(char)*reg->tamanhoNome+1);
		strcpy(reg->nomeLinha, nome);

		reg->tamanhoCor = strlen(cor);
		reg->corLinha = (char*)malloc(sizeof(char)*reg->tamanhoCor+1);
		strcpy(reg->corLinha, cor);
	}

	int funcionalidade8(FILE* fpBin, int qtdRegistros){
		// Le os registros e os adiciona no arquivo binario

		RegistroLinha* reg = criaRegistroLinha();
		HeaderLinha* h = leituraCabecalhoLinhaBinario(fpBin);
		if(h->status == '0'){
			// verificacao de status
			printf("Falha no processamento do arquivo.\n");
			return 0;
		}

		int cod = 0;
		char codNaoTratado[5];
		char cartao[1];
		char nome[100];
		char cor[100];

		for(int i = 0; i < qtdRegistros; i++){
			scanf("%s ", codNaoTratado);

			// Tratamento de registro removido
			if(codNaoTratado[0] == '*'){
				reg->removido = '0';
				codNaoTratado[0] = codNaoTratado[1];
				codNaoTratado[1] = codNaoTratado[2];
				codNaoTratado[2] = '\0';
			}else{
				reg->removido = '1';
			}
			cod = atoi(codNaoTratado);

			scan_quote_string(cartao);
			scan_quote_string(nome);
			scan_quote_string(cor);

			setRegistroLinha(reg, cod, cartao[0], nome, cor);

			//atualizaCabecalhoBinarioLinha(h, reg, fpBin);
			atualizaHeaderLinha(h, fpBin);
			h->byteProxReg += escreveRegistroLinha(reg, h->byteProxReg, fpBin);
			liberaRegistroLinha(reg);
			reg = criaRegistroLinha();

		}

		liberaRegistroLinha(reg);
		liberaCabecalhoLinha(h);

		return 1;
	}

