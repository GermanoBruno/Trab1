#ifndef VEICULO_H
#define VEICULO_H

typedef struct cabecalhoVeiculo HeaderVeiculo;
typedef struct registroVeiculo RegistroVeiculo;

HeaderVeiculo* criarHeader();
void readHeader(HeaderVeiculo* h, FILE* fp);
void descreveHeader(HeaderVeiculo* h, FILE* fp);
void setHeader(HeaderVeiculo* h, FILE* binario);
void atualizaHeader (HeaderVeiculo* h, FILE* binario);
void readHeaderBin(HeaderVeiculo* h, FILE* binario);
void solveRemoved(char* prefix);
void setVeiculo(RegistroVeiculo* v, char* prefixo, char* data, int lug, int codl, char* modelo, char* categoria);
int leitura (FILE* fp, RegistroVeiculo* v);
int escrita (RegistroVeiculo* v, char removido, int byteProxReg, FILE* binario);
void atualizaBinario(HeaderVeiculo* h, RegistroVeiculo* v, FILE* binario);
void imprime (RegistroVeiculo* v);
void libera (RegistroVeiculo* v);
void busca (FILE* binario, char* campo, int n);
//void binarioNaTela(char *nomeArquivoBinario);
int check(FILE* fp);
void func1(FILE* fp, FILE* binario);
void func3(FILE* binario);
void liberaHeader(HeaderVeiculo* h);
RegistroVeiculo* create();

void leituraBinario (FILE* binario, RegistroVeiculo* v, char* removido, int* tam);

void imprimeVeiculo(RegistroVeiculo* v);

void busca_prefixo (FILE* binario, char prefixo[100]);

void busca_data (FILE* binario, char data[100]);

void busca_qtdlug (FILE* binario, int qtdlug);

void busca_modelo (FILE* binario, char modelo[100]);

void busca_categoria (FILE* binario, char categoria[100]);

void func5(FILE* binario, char nomeDoCampo[17], char valor[100]);
void func7(FILE* binario, int n);

#endif
