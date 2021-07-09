#ifndef VEICULO_H
#define VEICULO_H

typedef struct PAGE page;
typedef struct cabecalhoVeiculo HeaderVeiculo;
typedef struct registroVeiculo RegistroVeiculo;

void writeHeader(FILE* bin, char status, int rootRRN, int lastRRN);
page* createPage();
int getRRN(page* pag); //TROCAR POR OUTRA COISA
void printPage(page* pag);
void readPage(FILE* bin, page* pag);
void printPage(page* pag);
void split(page* pag, int* promo_key, long int* promo_PR, int* promo_r_child, page* newpage);
int pageInsert(FILE* bin, int current_rrn, int key, long int PR, int* promo_r_child, int* promo_key, long int* promo_PR, int* last_rrn);
void btreeInsert(FILE* bin, page** root, int key, long int PR, int* RRN);
int searchKey(FILE* bin, int current_rrn, int key);
//void printBTree(FILE* bin, int current_rrn);
//void freeBTree(page* root);

//-------------------------------------------------------------------------------------------------

HeaderVeiculo* criarHeader();
// Função para debug, imprime os campos do cabeçalho h
void imprimeHeader(HeaderVeiculo* h);
// Função para ler do .csv os campos que descrevem as colunas de dados dos registros
void descreveHeader(HeaderVeiculo* h, FILE* fp);
// Função que insere os valores iniciais do cabeçalho h no arquivo binario.
// Usada no início da funcionalidade 1, quando estamos começando a transferir os dados.
void setHeader(HeaderVeiculo* h, FILE* binario);
// Função que lê os campos do cabeçalho 'h' de um arquivo binario  
void readHeaderBin(HeaderVeiculo* h, FILE* binario);
// Função que atualiza as informações do cabeçalho de um arquivo binário após a inserção dos registros
void atualizaHeader (HeaderVeiculo* h, FILE* binario);
// Função que libera a memória ocupada por um cabeçalho 'h'
void liberaHeader(HeaderVeiculo* h);

//-------------------------------------------------------------------------------------------------

RegistroVeiculo* create();
// Função que atribui campos fornecidos pelo usuário para um dado registro 'v'
void setVeiculo(RegistroVeiculo* v, char* prefixo, char* data, int lug, int codl, char* modelo, char* categoria);
// Função que lê os campos de um veículo a partir de um arquivo .csv
int leitura (FILE* fp, RegistroVeiculo* v);
// Função que escreve os campos de um dado registro 'v' num arquivo binario, a partir do byte 'byteProxReg'
int escrita (RegistroVeiculo* v, char removido, int byteProxReg, FILE* binario);
// Função maior de escrita no arquivo binário. Usa como base a função 'escrita' anterior.
void atualizaBinario(HeaderVeiculo* h, RegistroVeiculo* v, FILE* binario);
// Função que libera o espaço de memória ocupado por um registro 'v' e seus campos
void libera (RegistroVeiculo* v);
void imprimeVeiculo(RegistroVeiculo* v);
void leituraBinario (FILE* binario, RegistroVeiculo* v, char* removido, int* tam);
int getPrefix(RegistroVeiculo* v);






/*
void func1(FILE* fp, FILE* binario);
void func3(FILE* binario);
void func5(FILE* binario, char* nomeDoCampo, char* valor);
int func7(FILE* binario, int n);*/

#endif
