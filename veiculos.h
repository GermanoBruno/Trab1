#ifndef VEICULO_H
#define VEICULO_H

typedef struct cabecalhoVeiculo HeaderVeiculo;
typedef struct registroVeiculo  RegistroVeiculo;

header* createHeader();
void descreveHeader(header* h, FILE* fp);
void setHeader(header* h, FILE* binario);
void atualizaHeader (header* h, FILE* binario);
veiculo* create();
int leitura (FILE* fp, veiculo* v);
int escrita (veiculo* v, char removido, int byteProxReg, FILE* binario);
void atualizaBinario(header* h, veiculo* v, FILE* binario);
void imprime (veiculo* v);
void libera (veiculo* v);
void busca (FILE* binario, char* campo, int n);
void binarioNaTela(char *nomeArquivoBinario);


#endif