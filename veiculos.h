#ifndef VEICULO_H
#define VEICULO_H

typedef struct cabecalhoVeiculo HeaderVeiculo;
typedef struct registroVeiculo  RegistroVeiculo;

HeaderVeiculo* createHeader();
void descreveHeader(HeaderVeiculo* h, FILE* fp);
void setHeader(HeaderVeiculo* h, FILE* binario);
void atualizaHeader (HeaderVeiculo* h, FILE* binario);
RegistroVeiculo* create();
int leitura (FILE* fp, RegistroVeiculo* v);
int escrita (RegistroVeiculo* v, char removido, int byteProxReg, FILE* binario);
void atualizaBinario(HeaderVeiculo* h, RegistroVeiculo* v, FILE* binario);
void imprime (RegistroVeiculo* v);
void libera (RegistroVeiculo* v);
void busca (FILE* binario, char* campo, int n);
void binarioNaTela(char *nomeArquivoBinario);


#endif