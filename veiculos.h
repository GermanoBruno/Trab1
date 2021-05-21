#ifndef VEICULO_H
#define VEICULO_H

typedef struct Header header;
typedef struct Veiculo veiculo;

header* createHeader();
void descreveHeader(header* h, FILE* fp);
void setHeader(header* h, FILE* binario);
veiculo* create();
int leitura (FILE* fp, veiculo* v);
int escrita (veiculo* v, char removido, int byteProxReg, FILE* binario);
void atualizaBinario(header* h, veiculo* v, FILE* binario);
void imprime (veiculo* v);
void libera (veiculo* v);
void binarioNaTela(char *nomeArquivoBinario);


#endif