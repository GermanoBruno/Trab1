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

// Registros
	
RegistroLinha* criaRegistroLinha(void);

int leRegistroLinhaCsv(RegistroLinha* reg, FILE* fp);
void liberaRegistroLinha(RegistroLinha* reg);
void atualizaHeaderLinha (HeaderLinha* h, FILE* binario);
void atualizaCabecalhoBinarioLinha(HeaderLinha* h, RegistroLinha* reg, FILE* fp);
void imprimeRegistroLinha(RegistroLinha* reg);
int escreveRegistroLinha(RegistroLinha* reg, int byteProxReg, FILE* fp);
int leRegistroLinha(RegistroLinha* reg, FILE* fp);
void buscaRegistroLinha(FILE* fp);

// DEBUG PRINT
void printDebugRegistroLinha(RegistroLinha* reg);

int checkFile(FILE* fp); 

void buscaCartao(FILE* fp, char* valor);

void buscaNome(FILE* fp, char* valor);

void buscaCodigo(FILE* fp, char* valor);

void buscaCor(FILE* fp, char* valor);

void setRegistroLinha(RegistroLinha* reg, int cod, char cartao, char* nome, char* cor);


#endif