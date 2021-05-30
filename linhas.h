#ifndef LINHAS_H
#define LINHAS_H

#define DEBUG 0

typedef struct cabecalhoLinha HeaderLinha;
typedef struct registroLinha  RegistroLinha;

// Cabeçalho
HeaderLinha* criarHeaderLinha(void);

void leituraCabecalhoLinhaCsv(HeaderLinha* h, FILE* fp);

HeaderLinha* leituraCabecalhoLinhaBinario(FILE* fp);

void escritaCabecalhoLinha(HeaderLinha* h, FILE* fp);

void atualizaCabecalhoLinha(HeaderLinha*h, FILE* fp);

void liberaCabecalhoLinha(HeaderLinha* h);

void printDebugCabecalhoLinha(HeaderLinha* h);

	// DEBUG PRINT
	///////////////////////////////////////////////////////////////////////
// Registros
	
RegistroLinha* criaRegistroLinha(void);

int leRegistroLinhaCsv(RegistroLinha* reg, FILE* fp);
void liberaRegistroLinha(RegistroLinha* reg);
void atualizaCabecalhoBinarioLinha(HeaderLinha* h, RegistroLinha* reg, FILE* fp);
void imprimeRegistroLinha(RegistroLinha* reg);
int escreveRegistroLinha(RegistroLinha* reg, char removido, int byteProxReg, FILE* fp);
// AINDA NÃO TERMINEI A PARTIR DAQUI
int leRegistroLinha(RegistroLinha* reg, FILE* fp);
void buscaRegistroLinha(FILE* fp);

//////////////////////////////////////////////////////////////////////////
///// Escrita e busca tão copiados do veiculos pra referencia rapida /////
//////////////////////////////////////////////////////////////////////////


// DEBUG PRINT
void printDebugRegistroLinha(RegistroLinha* reg);

int checkFile(FILE* fp); 
// Funcionalidades
void funcionalidade2(FILE* fpCsv, FILE* fpBin);
void funcionalidade4(FILE* fpBin);
void funcionalidade6(FILE* fpBin, char nomeDoCampo[17], char valor[100]);
int funcionalidade8(FILE* fpBin, int qtdRegistros);

#endif