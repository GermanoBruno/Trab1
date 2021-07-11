#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "funcionalidades.h"
#include "linhas.h"
#include "veiculo.h"
#include "auxiliares.h"
#include "btree.h"

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




// Funcionalidade 1: Transferencia de dados do arquivo .csv para o binario
/*void funcionalidade1(FILE* fp, FILE* binario) {

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


// Funcionalidade 3: Imprimir os registros de um arquivo binario
// OBS: A função deveria ser mais compacta, já que as funções anteriores 'leituraBinario'
// e 'imprimeVeiculo' poderiam ser usadas para modularizar o codigo. Porém, não conseguimos
// achar a causa do bug que acontece quando usamos as funções, por isso deixamos o código assim.
void funcionalidade3(FILE* binario) {

	char removido, prefixo[6], data[11], *modelo, *categoria;
	int tam, qtdlug, tamodelo, tamcategoria, codlinha;
	HeaderVeiculo* h = criarHeader();

	readHeaderBin(h, binario);
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		liberaHeader(h);
		return;
	}

	if(h->nroRegistros == 0){
		printf("Registro inexistente.\n");
		liberaHeader(h);
		return;
	}

	// Enquanto não chegamos no fim do arquivo
	while (check(binario)) {
		fread(&(removido), sizeof(char), 1, binario);
		
		if (removido == '0') {
			fread(&(tam), sizeof(int), 1, binario);
			fseek(binario, tam, SEEK_CUR);
			continue;
			
		}  

		fread(&(tam), sizeof(int), 1, binario);

		fread(prefixo, sizeof(char), 5, binario);
		prefixo[5] = '\0';

		fread(data, sizeof(char), 10, binario);
		data[10] = '\0';

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
	liberaHeader(h);
}


// Funcionalidade 5: Busca por registros com campo 'nomeDoCampo' de valor 'valor'
void funcionalidade5(FILE* binario, char* nomeDoCampo, char* valor) {

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
int funcionalidade7(FILE* binario, int n) {

	HeaderVeiculo* h = criarHeader();

	readHeaderBin(h, binario);
	
	if(h->status == '0'){
		printf("Falha no processamento do arquivo.\n");
		liberaHeader(h);
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
}*/


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

int funcionalidade10(FILE* fpBin, FILE* fpIndex){
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
		return 1;
	}
	while(checkFile(fpBin)){
		pr = ftell(fpBin);
		leRegistroLinha(reg, fpBin);
		
		if(reg->removido) btreeInsert(fpIndex, &root, reg->codLinha, pr, &rrn);
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
	return 0;
}

void funcionalidade12(FILE* fpBin, FILE* fpIndex, int valor){
	HeaderLinha* h = leituraCabecalhoLinhaBinario(fpBin);
	if(h->status == '0'){
		// verificacao de status
		printf("Falha no processamento do arquivo.\n");
	}
	if(h->nroRegistros == 0){
		printf("Registro inexistente.\n");
		return;
	}
	RegistroLinha* reg;
	while (checkFile(fpBin)){
		// Verificando se o arquivo nao terminou
		reg = criaRegistroLinha();
		
		// Procura a chave na arvore
		long int pr = searchKey(fpIndex, 0, valor);
		if (pr == -1){
			// Não possui a chave
			printf("Registro inexistente.\n");
			return;
		}else{
			// Usa o PR para achar o registro
			fseek(fpBin, pr, SEEK_SET);
			leRegistroLinha(reg, fpBin);

			imprimeRegistroLinha(reg);
		}

		liberaRegistroLinha(reg);
	}
	liberaCabecalhoLinha(h);
}

int funcionalidade14(FILE* fpBin, FILE* fpIndex, int qtdRegistros){
	// Le os registros e os adiciona no arquivo binario
	char status;
	int rootRRN;
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
		if(reg->removido) btreeInsert(fpIndex, &root, reg->codLinha, pr, &rootRRN);

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