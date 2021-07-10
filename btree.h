#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERROR -1 		// valor de retorno da função pageInsert quando já existe um elemento a ser inserido
#define PROMOTION 1 	// valor de retorno da função pageInsert quando uma chave é promovida
#define NOPROMOTION 0 	// valor de retorno da função pageInsert quando não há promoção
// Estrutura da página de disco
typedef struct PAGE page;
// Função que insere as informações do cabeçalho do arquivo de índice
void writeHeader(FILE* bin, char status, int rootRRN, int proxRRN);
// Função de criação da página, assume que o nó é uma folha inicialmente
page* createPage();
// Função auxiliar que indica se uma página tem filhos ou não
int hasChilds(page* pag);
// Função auxiliar para resetar os valores de uma página, a fim de receber
// novos valores caso participe do split na inserção
void resetPage(page* pag);
// TROCAR POR OUTRA COISA!!!!!!!!!!!!!!!!
int getRRN(page* p);
// Função que escreve os valores de uma dada página no arquivo de índice
void writePage(FILE* bin, page* pag);

// Função que lê os valores de uma dada página do arquivo de índice
void readPage(FILE* bin, page* pag);

// Função auxiliar para imprimir os valores de uma dada página
void printPage(page* pag);

// Função split que participa do processo de inserção, para solucionar o caso de overflow numa página
void split(page* pag, int* promo_key, long int* promo_PR, int* promo_r_child, page* newpage);

// Função de inserção de uma chave no arquivo de índice. Recebe como parâmetros principais:
// |current_rrn| -> RRN atual da instância da função
// |key| -> chave a ser inserida
// |PR| -> ponteiro de referência da chave a ser inserida
// |last_rrn| -> variável auxiliar passada como referência para guardar o último RRN da btree, mantendo a sequência dos RRNs
int pageInsert(FILE* bin, int current_rrn, int key, long int PR, int* promo_r_child, int* promo_key, long int* promo_PR, int* last_rrn);
// Função principal de inserção de uma chave no arquivo de índice
// |RRN| -> variável auxiliar passada por referência para manter controle dos RRNs que vão se sucedendo na árvore
// |**root| -> ponteiro para página passado por referẽncia para atualização em caso de promoção
void btreeInsert(FILE* bin, page** root, int key, long int PR, int* RRN);

// Função para buscar uma dada chave no arquivo de índice
int searchKey(FILE* bin, int current_rrn, int key);

// Debug
void printBTree(FILE* bin, int current_rrn);