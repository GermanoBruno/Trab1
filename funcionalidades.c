#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linhas.h"
#include "auxiliares.h"

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
		liberaCabecalhoLinha(h);
		liberaRegistroLinha(reg);
		return;

	}
 	if(DEBUG){
		printf("Func4 criou struct\n");
	}	
	if(h->nroRegistros == 0){
		// Se não houver registros, não tem o que imprimir
		printf("Registro inexistente.\n");
		liberaCabecalhoLinha(h);
		liberaRegistroLinha(reg);
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

int funcionalidade8(FILE* fpBin, int qtdRegistros){
	// Le os registros e os adiciona no arquivo binario

	RegistroLinha* reg = criaRegistroLinha();
	HeaderLinha* h = leituraCabecalhoLinhaBinario(fpBin);
	if(h->status == '0'){
		// verificacao de status
		printf("Falha no processamento do arquivo.\n");
		liberaRegistroLinha(reg);
		liberaCabecalhoLinha(h);
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

		//int tamBonus = escreveRegistroLinha(reg, h->byteProxReg, fpBin);
		//h->byteProxReg += escreveRegistroLinha(reg, h->byteProxReg, fpBin);
		atualizaCabecalhoBinarioLinha(h, reg, fpBin);
		//atualizaHeaderLinha(h, fpBin);
		liberaRegistroLinha(reg);
		reg = criaRegistroLinha();

	}
	//atualizaCabecalhoBinarioLinha(h, reg, fpBin);
	atualizaHeaderLinha(h, fpBin);
	liberaRegistroLinha(reg);
	liberaCabecalhoLinha(h);

	return 1;
}

void funcionalidade10(FILE* fpBin, FILE* fpIndex){
	int rrn;
	long int pr;
	page* root = NULL;

	// Aloca um registro na memoria
	RegistroLinha* reg = criaRegistroLinha();
	// Lê o cabeçalho
 	HeaderLinha* h = leituraCabecalhoLinhaBinario(fpBin);

 	// Verificacao do status do arquivo
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		// Se 0, arquivo não foi finalizado corretamente
		// A leitura acaba
		liberaCabecalhoLinha(h);
		liberaRegistroLinha(reg);
		return;
	}
	while(checkFile(fpBin)){
		pr = ftell(fpBin);
		leRegistroLinha(reg, fpBin);
		
		if(reg->removido) btreeInsert(fpIndex, &root, reg->codLinha, pr, rrn);
		liberaRegistroLinha(reg);
		reg = criaRegistroLinha();
	}

	//atualizaHeaderLinha(h, fpBin);
	liberaCabecalhoLinha(h);
	liberaRegistroLinha(reg);
	free(root);
	if(DEBUG){
		printf("Func10 terminou\n");
	}
}

void funcionalidade12(FILE* fpBin, FILE* fpIndex){
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
		
		// Procura a chave na arvore
		long int pr = searchKey();
		if (pr == 0){
			// Não possui a chave
		}else{
			// USar o PR (resutado da busca) para achar o registro
			fseek(fpBin, pr, SEEK_SET);
			leRegistroLinha(reg, fpBin);

			imprimeRegistroLinha(reg);
		}

		liberaRegistroLinha(reg);
	}
	liberaCabecalhoLinha(h);
}

void funcionalidade14(FILE* fpBin, FILE* fpIndex, int qtdRegistros){
	// Le os registros e os adiciona no arquivo binario
	char status;
	int rootRRN
	int cod = 0;
	char codNaoTratado[5];
	char cartao[1];
	char nome[100];
	char cor[100];
	long int pr;
	page* root;


	RegistroLinha* reg = criaRegistroLinha();
	HeaderLinha* h = leituraCabecalhoLinhaBinario(fpBin);
	
	// leitura do status do arquivo de indices
	fread(&(status), sizeof(char), 1, fpIndex);  

	// verificacao de status dos arquivos
	if((h->status == '0') || (status == '0')){
		printf("Falha no processamento do arquivo.\n");
		liberaRegistroLinha(reg);
		liberaCabecalhoLinha(h);
		return 0;
	}

	// Leitura do RRN da raiz da arvore
	fread(&(rootRRN), sizeof(int), 1, fpIndex);
	// Ajuste do ponteiro da arvore para a posicao da raiz
	fseek(fpIndex, rootRRN, SEEK_SET);

	readPage(fpIndex, root);

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
		

		// Guarda em pr a posição onde o registro será inserido
		pr = ftell(fpBin);
		// Insere o registro no arquivo de dados e atualiza cabeçalho
		atualizaCabecalhoBinarioLinha(h, reg, fpBin);
		// Insere a chave na arvore
		if(reg->removido) btreeInsert(fpIndex, &root, reg->codLinha, pr, rrn);

		liberaRegistroLinha(reg);
		reg = criaRegistroLinha();

	}
	//atualizaCabecalhoBinarioLinha(h, reg, fpBin);
	atualizaHeaderLinha(h, fpBin);
	liberaRegistroLinha(reg);
	liberaCabecalhoLinha(h);
	free(root);

	return 1;
}