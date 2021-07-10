#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Estrutura da página de disco
struct PAGE {

	char leaf; // flag que indica se o nó é folha ou não
	int RRN; // RRN da página
	int count; // número de chaves armazenadas na página
	int C[4]; // vetor de chaves
	long int PR[4]; // vetor de ponteiros de referência para o arquivo de dados
	int P[5]; // vetor de filhos
};
// Função que insere as informações do cabeçalho do arquivo de índice
void writeHeader(FILE* bin, char status, int rootRRN, int proxRRN) {

	fseek(bin, 0, SEEK_SET);
	fwrite(&(status), sizeof(char), 1, bin); // status do arquivo
	fwrite(&(rootRRN), sizeof(int), 1, bin); // RRN da raiz
	fwrite(&(proxRRN), sizeof(int), 1, bin); // RRN do prox nó
	
	// O resto do espaço é reservado para o lixo
	char lixo = '@';
	for (int i = 0; i < 68; i++) {
		fwrite(&(lixo), sizeof(char), 1, bin);
	}
}
// Função de criação da página, assume que o nó é uma folha inicialmente
page* createPage() {

	page* pag = (page*)malloc(sizeof(page));
	pag->leaf = '1';
	pag->count = 0;
	pag->RRN = -1;

	// Os valores são inicializados por -1
	for (int i = 0; i < 4; i++) {
		pag->C[i] = -1;
		pag->PR[i] = -1;
		pag->P[i] = -1;
	}
	pag->P[4] = -1;

	return pag;
}
// Função auxiliar que indica se uma página tem filhos ou não
int hasChilds(page* pag) {

	for (int i = 0; i < pag->count+1; i++) {
		if (pag->P[i] != -1) return 1;
	}

	return 0;
}
// Função auxiliar para resetar os valores de uma página, a fim de receber
// novos valores caso participe do split na inserção
void resetPage(page* pag) {

	pag->count = 0;

	for (int i = 0; i < 4; i++) {
		pag->C[i] = -1;
		pag->PR[i] = -1;
		pag->P[i] = -1;
	}
	pag->P[4] = -1;
}
// TROCAR POR OUTRA COISA!!!!!!!!!!!!!!!!
int getRRN(page* p) {
	
	return p->RRN;
}
// Função que escreve os valores de uma dada página no arquivo de índice
void writePage(FILE* bin, page* pag) {

	fwrite(&(pag->leaf), sizeof(char), 1, bin);
	fwrite(&(pag->count), sizeof(int), 1, bin);
	fwrite(&(pag->RRN), sizeof(int), 1, bin);

	for (int i = 0; i < 4; i++) {
		fwrite(&(pag->P[i]), sizeof(int), 1, bin);
		fwrite(&(pag->C[i]), sizeof(int), 1, bin);
		fwrite(&(pag->PR[i]), sizeof(long int), 1, bin);
	}
	fwrite(&(pag->P[4]), sizeof(int), 1, bin);
}

// Função que lê os valores de uma dada página do arquivo de índice
void readPage(FILE* bin, page* pag) {

	fread(&(pag->leaf), sizeof(char), 1, bin);
	fread(&(pag->count), sizeof(int), 1, bin);
	fread(&(pag->RRN), sizeof(int), 1, bin);

	for (int i = 0; i < 4; i++) {
		fread(&(pag->P[i]), sizeof(int), 1, bin);
		fread(&(pag->C[i]), sizeof(int), 1, bin);
		fread(&(pag->PR[i]), sizeof(long int), 1, bin);
	}
	fread(&(pag->P[4]), sizeof(int), 1, bin);
}

// Função auxiliar para imprimir os valores de uma dada página
void printPage(page* pag) {

	int i;
	for (i = 0; i < pag->count; i++) {
		printf("%d %d %ld ", pag->P[i], pag->C[i], pag->PR[i]);
	}
	printf("%d", pag->P[i]);
	printf("\n");
}

// Função split que participa do processo de inserção, para solucionar o caso de overflow numa página
void split(page* pag, int* promo_key, long int* promo_PR, int* promo_r_child, page* newpage) {

	int keys[5]; // Vetor auxiliar de chaves com 4+1 espaços para ajustar as posições das chaves
	long int PRs[5]; // Vetor auxiliar de ponteiros de referência com 4+1 espaços para ajustar as posições dos ponteiros
	int childs[6]; // Vetor auxiliar de filhos com 5+1 posições para ajustar as posições dos filhos

	// Começamos inserindo os valores da página corrente 'p' nos vetores auxiliares
	for (int i = 0; i < 4; i++) {
		keys[i] = pag->C[i];
		PRs[i] = pag->PR[i];
		childs[i] = pag->P[i];
	}
	childs[4] = pag->P[4];


	// Buscamos/ a posição correta da nova chave no vetor auxiliar (consequentemente a do seu ponteiro de referência)
	int i = 3;
	while (i >= 0) {

		if (keys[i] < *promo_key) break;
		keys[i+1] = keys[i];
		PRs[i+1] = PRs[i];
		i--;
	}
	int pos = i+1;

	// Inserimos a chave e seu ponteiro de referência em suas posições corretas 
	keys[pos] = *promo_key;
	PRs[pos] = *promo_PR;

	// Insere os filhos nos lugares devidos, incluindo o da chave promovida
	for (int i = 4; i >= pos+1; i--) {
		childs[i+1] = childs[i];
	}
	childs[pos+1] = *promo_r_child;

	// Reinicializamos a página corrente para receber os novos valores das
	// chaves, ponteiros e filhos
	resetPage(pag);

	// Coloca as chaves que precedem a chave promovida, seus ponteiros e seus filhos na pagina corrente
	for (int i = 0; i < 2; i++) {
		pag->C[i] = keys[i];
		pag->PR[i] = PRs[i];
		pag->P[i] = childs[i];
	}
	pag->P[2] = childs[2];
	pag->count = 2;

	// Coloca as chaves que sucedem a chave promovida, seus ponteiros e seus filhos na nova pagina
	for (int i = 3; i < 5; i++) {
		newpage->C[i-3] = keys[i];
		newpage->PR[i-3] = PRs[i];
		newpage->P[i-3] = childs[i];
	}
	newpage->P[2] = childs[5];
	newpage->count = 2;

	// Se alguma das páginas tem filhos, significa que ela não é mais uma folha
	if (hasChilds(pag)) pag->leaf = '0';
	if (hasChilds(newpage)) newpage->leaf = '0';

	// Atualiza os dados da promoção: a chave do meio e seu ponteiro de referência, e o RRN do seu filho direito
	*promo_key = keys[2];
	*promo_PR = PRs[2];
	*promo_r_child = newpage->RRN;
}

// Função de inserção de uma chave no arquivo de índice. Recebe como parâmetros principais:
// |current_rrn| -> RRN atual da instância da função
// |key| -> chave a ser inserida
// |PR| -> ponteiro de referência da chave a ser inserida
// |last_rrn| -> variável auxiliar passada como referência para guardar o último RRN da btree, mantendo a sequência dos RRNs
int pageInsert(FILE* bin, int current_rrn, int key, long int PR, int* promo_r_child, int* promo_key, long int* promo_PR, int* last_rrn) {

	// Se o RRN atual é -1, significa que viemos de um nó folha,
	// entao promovemos a chave de volta para ele
	if (current_rrn == -1) {

		*promo_key = key;
		*promo_PR = PR;
		*promo_r_child = -1;

		return PROMOTION;
	}

	//Desloca o ponteiro do arquivo para o RRN atual
	fseek(bin, 77*(current_rrn+1), SEEK_SET);

	// Cria uma página auxiliar para ler os campos
	page* pag = createPage();

	// Variáveis auxiliares para incrementar a iteração, guardar a posicao correta de inserção
	// e guardar o valor de retorno da instância da função
	int i, pos, return_value;
	
	// Lê os campos
	readPage(bin, pag);

	// Percorre as chaves da página até uma das condições ser satisfeita:
	// 1) Achamos uma chave igual à candidata a inserção, logo retornamos ERRO
	// 2) Achamos uma chave maior que a candidata, logo temos a posicao de inserção, que é antes dessa chave
	for (i = 0; i < pag->count; i++) {
		if (pag->C[i] == key) return ERROR;
		else if (pag->C[i] > key) {
			break;
		}
	}
	pos = i;

	// Chamada recursiva para o nível mais abaixo, na posição correta de inserção da chave
	return_value = pageInsert(bin, pag->P[pos], key, PR, promo_r_child, promo_key, promo_PR, last_rrn);

	// Se não houve promoção ou se já existe um nó com a chave a ser inserida
	if (return_value == NOPROMOTION || return_value == ERROR) {

		// Libera a chave auxiliar
		free(pag);
		
		// Retorna essa informação para o nível mais acima
		return return_value;
	}

	// Se não, mas existe espaço na página atual
	else if (pag->count < 4) {

		// Shifta as keys e seus ponteiros
		for (int i = pag->count-1; i >= pos; i--) {
			pag->C[i+1] = pag->C[i];
			pag->PR[i+1] = pag->PR[i];
		}

		// Insere a chave promovida e seu ponteiro no devido lugar
		pag->C[pos] = *promo_key;
		pag->PR[pos] = *promo_PR;

		// Shifta os filhos
		for (int i = pag->count; i >= pos+1; i--) {
			pag->P[i+1] = pag->P[i];
		}
		// Insere o filho direito da chave no seu devido lugar
		pag->P[pos+1] = *promo_r_child;

		// Incrementa o número de keys da página
		pag->count++;		

		// Desloca o ponteiro para o RRN a ser escrito (o RRN atual) e então escreve
		fseek(bin, 77*(current_rrn+1), SEEK_SET);
		writePage(bin, pag);

		// Libera a página auxiliar
		free(pag);

		return NOPROMOTION;
	}

	// Senão, significa que a página está lotada e, portanto, precisamos executar o split
	else {

		// Cria nova página 
		page* newpage = createPage();

		// E seta seu RRN (uma unidade a mais que o último RRN gerado)
		(*last_rrn)++;
		newpage->RRN = *last_rrn;

		// Executa o algoritmo de split
		split(pag, promo_key, promo_PR, promo_r_child, newpage);
		
		// Escreve a página original atualizada
		fseek(bin, -77, SEEK_CUR);
		writePage(bin, pag);

		// Escreve a nova página no fim do arquivo
		fseek(bin, 0, SEEK_END);
		writePage(bin, newpage);

		// Libera as páginas auxiliares
		free(pag);
		free(newpage);

		return PROMOTION;
	}
}

// Função principal de inserção de uma chave no arquivo de índice
// |RRN| -> variável auxiliar passada por referência para manter controle dos RRNs que vão se sucedendo na árvore
// |**root| -> ponteiro para página passado por referẽncia para atualização em caso de promoção
void btreeInsert(FILE* bin, page** root, int key, long int PR, int* RRN) {

	int promo_key; // Variável auxiliar para guardar o valor da chave promovida
	long int promo_PR; // Variável auxiliar para guardar o valor do ponteiro de referência da chave promovida
	int promo_r_child; // Variável auxiliar para guardar o valor do filho direito da chave promovida

	// Se a raiz não existe, ou seja, a árvore está vazia
	if (*root == NULL) {

		// Criamos o nó raiz e setamos os valores passados como argumento para a função
		*root = createPage();
		(*root)->RRN = *RRN = 0;
		(*root)->C[0] = key;
		(*root)->PR[0] = PR;
		(*root)->count = 1;
		writePage(bin, *root);

	}

	// Senão, inserimos a chave e checamos se houve promoção 
	else if (pageInsert(bin, (*root)->RRN, key, PR, &promo_r_child, &promo_key, &promo_PR, RRN) == PROMOTION) {
		
		// Se houve promoção, criamos a nova raiz 
		page* newroot = createPage();
		newroot->count = 1;

		// Seu RRN é agora o mais recente da árvore
		(*RRN) += 1; newroot->RRN = *RRN;
		
		// A chave e seu ponteiro são os que foram promovidos
		newroot->C[0] = promo_key;
		newroot->PR[0] = promo_PR;

		// O filho esquerdo é a antiga raiz
		newroot->P[0] = (*root)->RRN;

		// E seu filho direito é o mesmo da chave promovida
		newroot->P[1] = promo_r_child;

		// Escreve a página nó fim do arquivo
		fseek(bin, 0, SEEK_END);
		writePage(bin, newroot);
		
		// A raiz é atualizada
		*root = newroot;

	}
}

// Função para buscar uma dada chave no arquivo de índice
int searchKey(FILE* bin, int current_rrn, int key) {

	// Se chegamos no fim, significa que não a chave procurada não existe
	if (current_rrn == -1) return 0;

	// Senão, criamos uma página auxiliar para ler os campos
	page* root = createPage();
	
	// Variável auxiliar
	int child;

	// Deslocamos o ponteiro para o RRN da página atual e lemos os dados
	fseek(bin, 77*(current_rrn+1), SEEK_SET);
	readPage(bin, root);

	int i;
	// Percorremos o vetor de chaves
	for (i = 0; i < root->count; i++) {

		// Se a chave está presente, retorna que achou
		if (root->C[i] == key) {

			// Libera a página auxiliar
			free(root);			
			return root->PR[i];
		}

		// Se a chave atual é maior que a chave procurada, então achamos a posição correta de busca para o próximo nível 
		else if (root->C[i] > key) {
			break;
		}
	}

	// Guarda o filho a ser buscado na varíavel auxiliar
	child = root->P[i];

	// Libera a página auxiliar
	free(root);

	// Busca no filho correto
	return searchKey(bin, child, key);	
}

// Debug
void printBTree(FILE* bin, int current_rrn) {

	if (current_rrn == -1) return; 

	page* root = createPage();
	int child;

	fseek(bin, 77*(current_rrn+1), SEEK_SET);
	readPage(bin, root);
	printPage(root);

	int i;
	for (i = 0; i < root->count+1; i++) {

		child = root->P[i];
		printBTree(bin, child);
	
	}

	free(root);

	return;
}