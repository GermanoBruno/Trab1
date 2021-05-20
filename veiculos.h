#ifndef VEICULO_H
#define VEICULO_H

typedef struct Header header;
typedef struct Veiculo veiculo;

veiculo* create();
int leitura (FILE* fp, veiculo* v);
void escrita (veiculo* v, FILE* binario);
void imprime (veiculo* v);
void libera(veiculo* v);
void binarioNaTela(char *nomeArquivoBinario);


#endif