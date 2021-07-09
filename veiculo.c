#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "veiculo.h"
#include "auxiliares.h"

<<<<<<< HEAD
#define ERROR -1 // valor de retorno da função pageInsert quando já existe um elemento a ser inserido
#define PROMOTION 1 // valor de retorno da função pageInsert quando uma chave é promovida
#define NOPROMOTION 0 // valor de retorno da função pageInsert quando não há promoção

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
			return 1;
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

//-----------------------------------------------------------------------------------------------------

// Cabeçalho do arquivo de veiculos, contendo seus campos
struct cabecalhoVeiculo {

	char status; // status do arquivo de dados
	long int byteProxReg; // byte do próximo registro a ser inserido
	int nroRegistros; // número de registros válidos
	int nroRegistrosRemovidos; // número de registros removidos

	char descrevePrefixo[19]; // campo de referência csv
	char descreveData[36]; // campo de referência csv
	char descreveLugares[43]; // campo de referência csv
	char descreveLinha[27]; // campo de referência csv 
	char descreveModelo[18]; // campo de referência csv
	char descreveCategoria[21]; // campo de referência csv
};

// Função que cria um ponteiro para cabeçalho de veiculo e inicializa seus campos
HeaderVeiculo* criarHeader() {
	HeaderVeiculo* h = (HeaderVeiculo*)malloc(sizeof(HeaderVeiculo));
	h->status = '0';
	h->byteProxReg = 0;
	h->nroRegistros = 0;
	h->nroRegistrosRemovidos = 0;
}

// Função para debug, imprime os campos do cabeçalho h
void imprimeHeader(HeaderVeiculo* h){
	printf("Status: %c\n", h->status);
	printf("byteProxReg: %ld\n", h->byteProxReg);
	printf("nroRegistros: %d\n", h->nroRegistros);
	printf("nroRegistrosRemovidos: %d\n", h->nroRegistrosRemovidos);
	printf("descrevePrefixo: %s\n", h->descrevePrefixo);
	printf("descreveData: %s\n", h->descreveData);
	printf("descreveLinha: %s\n", h->descreveLinha);
	printf("descreveModelo: %s\n", h->descreveModelo);
	printf("descreveCategoria: %s\n", h->descreveCategoria);
}

// Função para ler do .csv os campos que descrevem as colunas de dados dos registros
void descreveHeader(HeaderVeiculo* h, FILE* fp) {
	fscanf(fp, "%[^,],", h->descrevePrefixo);
	fscanf(fp, "%[^,],", h->descreveData);
	fscanf(fp, "%[^,],", h->descreveLugares);
	fscanf(fp, "%[^,],", h->descreveLinha);
	fscanf(fp, "%[^,],", h->descreveModelo);
	fscanf(fp, "%[^\n]\n", h->descreveCategoria);
}

// Função que insere os valores iniciais do cabeçalho h no arquivo binario.
// Usada no início da funcionalidade 1, quando estamos começando a transferir os dados.
void setHeader(HeaderVeiculo* h, FILE* binario) {

	// Insere os campos utilitários do arquivo
	fwrite(&(h->status), sizeof(char), 1, binario);
	fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
	fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
	fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);

	// Insere os campos de referência do .csv
	fwrite(h->descrevePrefixo, sizeof(char), 18, binario);
	fwrite(h->descreveData, sizeof(char), 35, binario);
	fwrite(h->descreveLugares, sizeof(char), 42, binario);
	fwrite(h->descreveLinha, sizeof(char), 26, binario);
	fwrite(h->descreveModelo, sizeof(char), 17, binario);
	fwrite(h->descreveCategoria, sizeof(char), 20, binario);

	// Seta o byte do próximo registro (tamanho do header == 175 bytes)
	h->byteProxReg = 175;
}

// Função que lê os campos do cabeçalho 'h' de um arquivo binario  
void readHeaderBin(HeaderVeiculo* h, FILE* binario) {

	// Lê os campos utilitários do arquivo
    fread(&(h->status), sizeof(char), 1, binario);
    fread(&(h->byteProxReg), sizeof(long int), 1, binario);
    fread(&(h->nroRegistros), sizeof(int), 1, binario);
    fread(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);

    // Lê os campos de referência do .csv
    fread(h->descrevePrefixo, sizeof(char), 18, binario); 
    fread(h->descreveData, sizeof(char), 35, binario);
    fread(h->descreveLugares, sizeof(char), 42, binario);
    fread(h->descreveLinha, sizeof(char), 26, binario);
    fread(h->descreveModelo, sizeof(char), 17, binario);
    fread(h->descreveCategoria, sizeof(char), 20, binario);
}

// Função que atualiza as informações do cabeçalho de um arquivo binário após a inserção dos registros
void atualizaHeader (HeaderVeiculo* h, FILE* binario) {

	// Desloca para o começo do arquivo
	fseek(binario, 0, SEEK_SET);

	// Seta o status para 1, indicando que o arquivo fechou corretamente
	h->status = '1';

	// Escreve os campos utilitários
	fwrite(&(h->status), sizeof(char), 1, binario);
	fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
	fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
	fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
}

// Função que libera a memória ocupada por um cabeçalho 'h'
void liberaHeader(HeaderVeiculo* h) {

    if (h != NULL) free(h);
}

//---------------------------------------------------------------------------------------------------------

// Struct do registro veículo, contendo seus campos
struct registroVeiculo {

	char prefixo[6]; // prefixo do veículo
	char data[11]; // data de entrada do veículo na frota
	int quatidadeLugares; // quantidade de lugares no veículo
	int codLinha; // código de linha do veículo
	char* modelo; // modelo do veículo (tamanho variável)
	char* categoria; // categoria do veículo (tamanho variável)
	char removido; // campo que indica se o registro está removido ou não
	int tamanhoRegistro; // tamanho do registro no arquivo de dados
	int tamanhoModelo; // tamanho do campo 'modelo'
	int tamanhoCategoria; // tamanho do campo 'categoria'
};

// Função que cria um ponteiro para veículo 'v', já inicializando seus campos
RegistroVeiculo* create() {

	RegistroVeiculo *v = (RegistroVeiculo*)malloc(sizeof(RegistroVeiculo));
	v->modelo = NULL;
	v->categoria = NULL;
	v->quatidadeLugares = 0;
	v->codLinha = 0;
	v->removido = '0';
	v->tamanhoRegistro = 0;
	v->tamanhoModelo = 0;
	v->tamanhoCategoria = 0;

	return v;
}

int convertePrefixo(char* str) {

    /* O registro que tem essa string como chave foi removido */
    if(str[0] == '*')
        return -1;

    /* Começamos com o primeiro digito na ordem de 36^0 = 1 */
    int power = 1;

    /* Faz a conversão char por char para chegar ao resultado */
    int result = 0;
    for(int i = 0; i < 5; i++) {

        /* 
            Interpreta o char atual como se fosse um digito
            em base 36. Os digitos da base 36 são:
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D,
            E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
            S, T, U, V, W, X, Y, Z
        */
        int cur_digit;
        /* Checa pelos digitos normais e os converte para números */
        if(str[i] >= '0' && str[i] <= '9')
            cur_digit = str[i] - '0';
        /* Checa pelas letras e as converte para números */
        else if(str[i] >= 'A' && str[i] <= 'Z')
            cur_digit = 10 + str[i] - 'A';

        /*
            Multiplica o digito atual pelo ordem da posição atual
            e adiciona no resultado
            Primeira posição:   36^0 = 1
            Segunda posição:    36^1 = 36
            Terceira posição:   36^2 = 1.296
            Quarta posição:     36^3 = 46.656
            Quinta posição:     36^4 = 1.679.616
        */
        result += cur_digit * power;

        /* Aumenta a ordem atual */
        power *= 36;

    }

    return result;

}

int getPrefix(RegistroVeiculo* v) {
	return convertePrefixo(v->prefixo);
}

// Função auxiliar para tratar o caso de registros removidos (com prefixo começando por *)
void solveRemoved(char* prefix) {
	int tam = strlen(prefix);

	// Desloca as letras para sobrescrever o asterisco
	for (int i = 0; i < tam-1; i++) {
		prefix[i] = prefix[i+1];
	}
	prefix[tam-1] = '\0';
}

// Função que atribui campos fornecidos pelo usuário para um dado registro 'v'
void setVeiculo(RegistroVeiculo* v, char* prefixo, char* data, int lug, int codl, char* modelo, char* categoria) {

    strcpy(v->prefixo, prefixo);
    strcpy(v->data, data);
    v->quatidadeLugares = lug;
    v->codLinha = codl;

    v->modelo = (char*)malloc(sizeof(char)*strlen(modelo)+1);
    strcpy(v->modelo, modelo);
    v->modelo[strlen(modelo)] = '\0';
    v->tamanhoModelo = strlen(v->modelo);

    v->categoria = (char*)malloc(sizeof(char)*strlen(categoria)+1);
    strcpy(v->categoria, categoria);
    v->categoria[strlen(categoria)] = '\0';
    v->tamanhoCategoria = strlen(v->categoria);

}

// Função que lê os campos de um veículo a partir de um arquivo .csv
int leitura (FILE* fp, RegistroVeiculo* v) {

    char lixo[5]; // Variável auxiliar para controle dos dados de leitura

    fscanf(fp, "%[^,],", v->prefixo);
    fscanf(fp, "%[^,],", v->data);
    
    fscanf(fp, "%[^,],", lixo);

    // Se há NULO no lugar do campo, ele recebe -1
    if (strcmp(lixo, "NULO") == 0) v->quatidadeLugares = -1;
    else {
        v->quatidadeLugares = atoi(lixo);
    }

    fscanf(fp, "%[^,],", lixo);

    // Se há NULO no lugar do campo, ele recebe -1
    if (strcmp(lixo, "NULO") == 0) v->codLinha = -1;
    else {
        v->codLinha = atoi(lixo);
    }
    
    fscanf(fp, "%m[^,],", &(v->modelo));
    fscanf(fp, "%m[^\n]\n", &(v->categoria));

    return 1;
}

// Função que escreve os campos de um dado registro 'v' num arquivo binario, a partir do byte 'byteProxReg'
int escrita (RegistroVeiculo* v, char removido, int byteProxReg, FILE* binario) {

    int flagm = 0; // Flag que indica se o campo 'modelo' é nulo ou não
    int flagc = 0; // Flag que indica se o campo 'categoria' é nulo ou não
    int tam = 31; // Tamanho do registro

    // Se há NULO, marcamos isso para usar no código mais à frente
    if (strcmp(v->modelo, "NULO") == 0) {
        flagm = 1;
    } 
    // Senão, adicionamos o tamanho do campo ao tamanho do registro 
    else {
        v->tamanhoModelo = strlen(v->modelo);
        tam += v->tamanhoModelo;
    }

    // O mesmo aqui
    if (strcmp(v->categoria, "NULO") == 0) {
        flagc = 1;
    } 
    else {
        v->tamanhoCategoria = strlen(v->categoria);
        tam += v->tamanhoCategoria;
    }

    // Desloca-se o ponteiro para o byte em que começaremos a inserir
    fseek(binario, byteProxReg, SEEK_SET);

    // Escreve o campo 'removido', o tamanho do registro e seu prefixo
    fwrite(&(removido), sizeof(char), 1, binario);
    fwrite(&(tam), sizeof(int), 1, binario);
    fwrite(v->prefixo, sizeof(char), 5, binario);

    // --------- A partir daqui começam as verificações de validade ou não dos campos ------

    // Se campo 'data' é 'NULO'
    if (v->data[0] == 'N') {

    	// Escreve "\0@@@@..."

        char lixo = '\0';
        fwrite(&lixo, sizeof(char), 1, binario);
        
        lixo = '@';
        for (int i = 0; i < 9; i++) {
            fwrite(&lixo, sizeof(char), 1, binario); 
        }

    } else { // Se não, escreve a data de entrada
        fwrite(v->data, sizeof(char), strlen(v->data), binario); 
    }

    fwrite(&(v->quatidadeLugares), sizeof(int), 1, binario);
    fwrite(&(v->codLinha), sizeof(int), 1, binario); 

    // Se o modelo é nulo
    if (flagm == 1) {

    	// O tamanho do campo recebe 0 e não escreve o modelo
        int tam = 0;
        fwrite(&tam, sizeof(int), 1, binario);
    } else {

        fwrite(&(v->tamanhoModelo), sizeof(int), 1, binario);
        fwrite(v->modelo, sizeof(char), v->tamanhoModelo, binario); 
    }

    // Se a categoria é nula
    if (flagc == 1) {

    	// O tamanho do campo recebe 0 e não escreve a categoria
        int tam = 0;
        fwrite(&tam, sizeof(int), 1, binario);
    } else {
        fwrite(&(v->tamanhoCategoria), sizeof(int), 1, binario);
        fwrite(v->categoria, sizeof(char), v->tamanhoCategoria, binario); 
    }

    // Retornamos o tamanho do registro somado aos bytes de 'removido' e 'tamanho do registro'
    // para deslocar corretamente o ponteiro na próxima inserção
    int total = tam + 5;

    return total;
}

// Função maior de escrita no arquivo binário. Usa como base a função 'escrita' anterior.
void atualizaBinario(HeaderVeiculo* h, RegistroVeiculo* v, FILE* binario) {

	// Variáveis auxiliares
	int tam;
	char removido;

	// Se o registro foi removido
	if (v->prefixo[0] == '*') {

		// Corrige o prefixo
		solveRemoved(v->prefixo);

		// Incrementa o número de registros removidos
		h->nroRegistrosRemovidos++;

		// Atualiza o campo removido do registro
		removido = '0'; 
	}
	
	// Se o registro é válido
	else  {

		// Incrementa o número de registros válidos
		h->nroRegistros++;

		// Atualiza o campo removido do registro
		removido = '1';
	}

	// Escreve o registro com seu estado de remoção e recebe de volta 
	// o tamanho do registro completo (contendo 'removido' e 'tamanho do registro')
	tam = escrita(v, removido, h->byteProxReg, binario);

	// Atualiza o byte do próximo registro a ser inserido
	h->byteProxReg = h->byteProxReg + tam;
	
}
=======
// Header
	// Cabeçalho do arquivo de veiculos, contendo seus campos
	struct cabecalhoVeiculo {

		char status;
		long int byteProxReg;
		int nroRegistros;
		int nroRegistrosRemovidos;
		char descrevePrefixo[19];
		char descreveData[36];
		char descreveLugares[43];
		char descreveLinha[27];
		char descreveModelo[18];
		char descreveCategoria[21];
	};

	// Função que cria um ponteiro para cabeçalho de veiculo e inicializa seus campos
	HeaderVeiculo* criarHeader() {
		HeaderVeiculo* h = (HeaderVeiculo*)malloc(sizeof(HeaderVeiculo));
		h->status = '0';
		h->byteProxReg = 0;
		h->nroRegistros = 0;
		h->nroRegistrosRemovidos = 0;
	}

	// Função para debug, imprime os campos do cabeçalho h
	void imprimeHeader(HeaderVeiculo* h){
		printf("Status: %c\n", h->status);
		printf("byteProxReg: %ld\n", h->byteProxReg);
		printf("nroRegistros: %d\n", h->nroRegistros);
		printf("nroRegistrosRemovidos: %d\n", h->nroRegistrosRemovidos);
		printf("descrevePrefixo: %s\n", h->descrevePrefixo);
		printf("descreveData: %s\n", h->descreveData);
		printf("descreveLinha: %s\n", h->descreveLinha);
		printf("descreveModelo: %s\n", h->descreveModelo);
		printf("descreveCategoria: %s\n", h->descreveCategoria);
	}

	// Função para ler do .csv os campos que descrevem as colunas de dados dos registros
	void descreveHeader(HeaderVeiculo* h, FILE* fp) {
		fscanf(fp, "%[^,],", h->descrevePrefixo);
		fscanf(fp, "%[^,],", h->descreveData);
		fscanf(fp, "%[^,],", h->descreveLugares);
		fscanf(fp, "%[^,],", h->descreveLinha);
		fscanf(fp, "%[^,],", h->descreveModelo);
		fscanf(fp, "%[^\n]\n", h->descreveCategoria);
	}

	// Função que insere os valores iniciais do cabeçalho h no arquivo binario.
	// Usada no início da funcionalidade 1, quando estamos começando a transferir os dados.
	void setHeader(HeaderVeiculo* h, FILE* binario) {

		fwrite(&(h->status), sizeof(char), 1, binario);
		fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
		fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
		fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
		fwrite(h->descrevePrefixo, sizeof(char), 18, binario);
		fwrite(h->descreveData, sizeof(char), 35, binario);
		fwrite(h->descreveLugares, sizeof(char), 42, binario);
		fwrite(h->descreveLinha, sizeof(char), 26, binario);
		fwrite(h->descreveModelo, sizeof(char), 17, binario);
		fwrite(h->descreveCategoria, sizeof(char), 20, binario);

		h->byteProxReg = 175;
	}

	// Função que lê os campos do cabeçalho 'h' de um arquivo binario  
	void readHeaderBin(HeaderVeiculo* h, FILE* binario) {

	    fread(&(h->status), sizeof(char), 1, binario);
	    fread(&(h->byteProxReg), sizeof(long int), 1, binario);
	    fread(&(h->nroRegistros), sizeof(int), 1, binario);
	    fread(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
	    fread(h->descrevePrefixo, sizeof(char), 18, binario); 
	    fread(h->descreveData, sizeof(char), 35, binario);
	    fread(h->descreveLugares, sizeof(char), 42, binario);
	    fread(h->descreveLinha, sizeof(char), 26, binario);
	    fread(h->descreveModelo, sizeof(char), 17, binario);
	    fread(h->descreveCategoria, sizeof(char), 20, binario);
	}

	// Função que atualiza as informações do cabeçalho de um arquivo binário após a inserção dos registros
	void atualizaHeader (HeaderVeiculo* h, FILE* binario) {

		fseek(binario, 0, SEEK_SET);
		h->status = '1';
		fwrite(&(h->status), sizeof(char), 1, binario);
		fwrite(&(h->byteProxReg), sizeof(long int), 1, binario);
		fwrite(&(h->nroRegistros), sizeof(int), 1, binario);
		fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 1, binario);
	}

	// Função que libera a memória ocupada por um cabeçalho 'h'
	void liberaHeader(HeaderVeiculo* h) {

	    if (h != NULL) free(h);
	}

// Registro
	// Struct do registro veículo, contendo seus campos
	struct registroVeiculo {

		char prefixo[6];
		char data[11];
		int quatidadeLugares;
		int codLinha;
		char* modelo;
		char* categoria;
		char removido;
		int tamanhoRegistro;
		int tamanhoModelo;
		int tamanhoCategoria;
	};

	// Função que cria um ponteiro para veículo 'v', já inicializando seus campos
	RegistroVeiculo* create() {

		RegistroVeiculo *v = (RegistroVeiculo*)malloc(sizeof(RegistroVeiculo));
		v->modelo = NULL;
		v->categoria = NULL;
		v->quatidadeLugares = 0;
		v->codLinha = 0;
		v->removido = '0';
		v->tamanhoRegistro = 0;
		v->tamanhoModelo = 0;
		v->tamanhoCategoria = 0;
		return v;
	}

	// Função auxiliar para tratar o caso de registros removidos (com prefixo começando por *)
	void solveRemoved(char* prefix) {
		int tam = strlen(prefix);

		for (int i = 0; i < tam-1; i++) {
			prefix[i] = prefix[i+1];
		}
		prefix[tam-1] = '\0';
	}

	// Função que atribui campos fornecidos pelo usuário para um dado registro 'v'
	void setVeiculo(RegistroVeiculo* v, char* prefixo, char* data, int lug, int codl, char* modelo, char* categoria) {

	    strcpy(v->prefixo, prefixo);
	    strcpy(v->data, data);
	    v->quatidadeLugares = lug;
	    v->codLinha = codl;

	    v->modelo = (char*)malloc(sizeof(char)*strlen(modelo)+1);
	    strcpy(v->modelo, modelo);
	    v->modelo[strlen(modelo)] = '\0';

	    v->categoria = (char*)malloc(sizeof(char)*strlen(categoria)+1);
	    strcpy(v->categoria, categoria);
	    v->categoria[strlen(categoria)] = '\0';

	}

	// Função que lê os campos de um veículo a partir de um arquivo .csv
	int leitura (FILE* fp, RegistroVeiculo* v) {

	    char lixo[5];
	    fscanf(fp, "%[^,],", v->prefixo);
	    fscanf(fp, "%[^,],", v->data);
	    
	    fscanf(fp, "%[^,],", lixo);
	    // Se há NULO no lugar do campo, ele recebe -1
	    if (strcmp(lixo, "NULO") == 0) v->quatidadeLugares = -1;
	    else {
	        v->quatidadeLugares = atoi(lixo);
	    }

	    fscanf(fp, "%[^,],", lixo);
	    // Se há NULO no lugar do campo, ele recebe -1
	    if (strcmp(lixo, "NULO") == 0) v->codLinha = -1;
	    else {
	        v->codLinha = atoi(lixo);
	    }
	    
	    fscanf(fp, "%m[^,],", &(v->modelo));
	    fscanf(fp, "%m[^\n]\n", &(v->categoria));

	    return 1;
	}

	// Função que escreve os campos de um dado registro 'v' num arquivo binario, a partir do byte 'byteProxReg'
	int escrita (RegistroVeiculo* v, char removido, int byteProxReg, FILE* binario) {

	    int flagm = 0;
	    int flagc = 0;
	    int tam = 31;

	    // Se há NULO, marcamos isso para usar no código mais à frente
	    if (strcmp(v->modelo, "NULO") == 0) {
	        flagm = 1;
	    } 
	    // Senão, adicionamos o tamanho do campo ao tamanho do registro 
	    else {
	        v->tamanhoModelo = strlen(v->modelo);
	        tam += v->tamanhoModelo;
	    }

	    // O mesmo aqui
	    if (strcmp(v->categoria, "NULO") == 0) {
	        flagc = 1;
	    } 
	    else {
	        v->tamanhoCategoria = strlen(v->categoria);
	        tam += v->tamanhoCategoria;
	    }

	    // Desloca-se o ponteiro para o byte em que começaremos a inserir
	    fseek(binario, byteProxReg, SEEK_SET);

	    fwrite(&(removido), sizeof(char), 1, binario);
	    fwrite(&(tam), sizeof(int), 1, binario);
	    fwrite(v->prefixo, sizeof(char), 5, binario);

	    // A partir daqui começam as verificações de validade ou não dos campos,
	    // por isso as flags marcadas anteriormente
	    if (v->data[0] == 'N') {

	        char lixo = '\0';
	        fwrite(&lixo, sizeof(char), 1, binario);
	        
	        lixo = '@';
	        for (int i = 0; i < 9; i++) {
	            fwrite(&lixo, sizeof(char), 1, binario); 
	        }

	    } else {
	        fwrite(v->data, sizeof(char), strlen(v->data), binario); 
	    }

	    fwrite(&(v->quatidadeLugares), sizeof(int), 1, binario);

	    fwrite(&(v->codLinha), sizeof(int), 1, binario); 

	    if (flagm == 1) {
	        int tam = 0;
	        fwrite(&tam, sizeof(int), 1, binario);
	    } else {

	        fwrite(&(v->tamanhoModelo), sizeof(int), 1, binario);
	        fwrite(v->modelo, sizeof(char), v->tamanhoModelo, binario); 
	    }

	    if (flagc == 1) {
	        int tam = 0;
	        fwrite(&tam, sizeof(int), 1, binario);
	    } else {
	        fwrite(&(v->tamanhoCategoria), sizeof(int), 1, binario);
	        fwrite(v->categoria, sizeof(char), v->tamanhoCategoria, binario); 
	    }

	    // Retornamos o tamanho do registro somado aos bytes de 'removido' e 'tamanho do registro'
	    // para deslocar corretamente o ponteiro na próxima inserção
	    int total = tam + 5;

	    return total;
	}

	// Função maior de escrita no arquivo binário. Usa como base a função 'escrita' anterior.
	void atualizaBinario(HeaderVeiculo* h, RegistroVeiculo* v, FILE* binario) {

		int tam;
		char removido;

		if (v->prefixo[0] == '*') {
			solveRemoved(v->prefixo);
			h->nroRegistrosRemovidos++;
			removido = '0'; 
		}
		else  {
			h->nroRegistros++;
			removido = '1';
		}

		tam = escrita(v, removido, h->byteProxReg, binario);

		h->byteProxReg = h->byteProxReg + tam;
		
	}
>>>>>>> master

	// Função que libera o espaço de memória ocupado por um registro 'v' e seus campos
	void libera (RegistroVeiculo* v) {

		if (v != NULL) {
			if (v->modelo != NULL) free(v->modelo);
			if (v->categoria != NULL) free(v->categoria);
			free(v);
		
		}
	}

	// Função auxiliar que checa se chegamos ao fim do arquivo
	int check(FILE* fp) {
		
		char check;
	    
	    if(fread(&check, sizeof(char), 1, fp) != 1) return 0;
	    
	    fseek(fp, -sizeof(char), SEEK_CUR);

	   	return 1;
	}

// Funcoes Auxiliares

	// Função auxiliar que traduz o formato da data lida (AAAA-MM-DD) para o exigido pelo trabalho 
	void printData(char* data) {


		char ano[5], mes[3], dia[3];

		for (int i = 0; i < 4; i++) {
			ano[i] = data[i];
		}
		ano[4] = '\0';

		for (int i = 0, j = 5; j < 7; i++, j++) {
			mes[i] = data[j];
		}
		mes[2] = '\0';

		for (int i = 0, j = 8; j < 10; i++, j++) {
			dia[i] = data[j];
		}
		dia[2] = '\0';

		int m = atoi(mes);

		switch (m) {

			case 1:
				printf("%s de janeiro de %s\n", dia, ano);
				break;
			case 2:
				printf("%s de fevereiro de %s\n", dia, ano);
				break;
			case 3:
				printf("%s de março de %s\n", dia, ano);
				break;
			case 4:
				printf("%s de abril de %s\n", dia, ano);
				break;
			case 5:
				printf("%s de maio de %s\n", dia, ano);
				break;
			case 6:
				printf("%s de junho de %s\n", dia, ano);
				break;
			case 7:
				printf("%s de julho de %s\n", dia, ano);
				break;
			case 8:
				printf("%s de agosto de %s\n", dia, ano);
				break;
			case 9:
				printf("%s de setembro de %s\n", dia, ano);
				break;
			case 10:
				printf("%s de outubro de %s\n", dia, ano);
				break;
			case 11:
				printf("%s de novembro de %s\n", dia, ano);
				break;
			case 12:
				printf("%s de dezembro de %s\n", dia, ano); 
				break;

		}
	}

	// Função auxiliar para ler os campos de um registro 'v' a partir de um arquivo binário
	void leituraBinario (FILE* binario, RegistroVeiculo* v, char* removido, int* tam) {

		fread(removido, sizeof(char), 1, binario);
			
			// Se o registro está removido, deslocamos o ponteiro para o próximo registro
			if (*removido == '0') {
				
				fread(tam, sizeof(int), 1, binario);
				fseek(binario, *tam, SEEK_CUR);
				return;
				
			}  

			// Daqui em diante lemos os campos do registro
			fread(tam, sizeof(int), 1, binario);

			fread(v->prefixo, sizeof(char), 5, binario);
			v->prefixo[5] = '\0';

			fread(v->data, sizeof(char), 10, binario);
			v->data[10] = '\0';

			fread(&(v->quatidadeLugares), sizeof(int), 1, binario);

			fread(&(v->codLinha), sizeof(int), 1, binario);

			fread(&(v->tamanhoModelo), sizeof(int), 1, binario);
			if (v->tamanhoModelo != 0) {
				
				v->modelo = (char*)malloc(sizeof(char) * v->tamanhoModelo + 1); 
				fread(v->modelo, sizeof(char), v->tamanhoModelo, binario);
				v->modelo[v->tamanhoModelo] = '\0';
			
			}

			fread(&(v->tamanhoCategoria), sizeof(int), 1, binario);
			if (v->tamanhoCategoria != 0) {
				
				v->categoria = (char*)malloc(sizeof(char) * v->tamanhoCategoria + 1); 
				fread(v->categoria, sizeof(char), v->tamanhoCategoria, binario);
				v->categoria[v->tamanhoCategoria] = '\0';
			
			}
	}

	// Função para imprimir os campos de um dado veículo 'v'
	void imprimeVeiculo(RegistroVeiculo* v) {

		printf("Prefixo do veiculo: %s\n", v->prefixo);

		printf("Modelo do veiculo: ");
		if (v->tamanhoModelo) printf("%s\n", v->modelo);
		else printf("campo com valor nulo\n");

		printf("Categoria do veiculo: ");
		if (v->tamanhoCategoria) printf("%s\n", v->categoria);
		else printf("campo com valor nulo\n");

		printf("Data de entrada do veiculo na frota: ");
		if (v->data[0] == '\0') printf("campo com valor nulo\n");
		else printData(v->data);

		if (v->quatidadeLugares != -1) printf("Quantidade de lugares sentados disponiveis: %d\n", v->quatidadeLugares);
		else printf("campo com valor nulo\n");
			
		printf("\n");
	}

	// Função auxiliar da funcionalidade 5 que busca por registros de prefixo 'prefixo'
	void busca_prefixo (FILE* binario, char* prefixo) {

		char removido; int tam;
		HeaderVeiculo* h = criarHeader();

		readHeaderBin(h, binario);

		if(h->status == '0'){
			printf("Falha no processamento do arquivo.\n");
			return;
		}
		
		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}

		RegistroVeiculo *v;
		while (check(binario)){

			v = create();
			leituraBinario(binario, v, &removido, &tam);
			
			if (!removido) {
				libera(v);
				continue;
			}

			if (strcmp(v->prefixo, prefixo) == 0) imprimeVeiculo(v);

			libera(v);

		}

		liberaHeader(h);
	}

	// Função auxiliar da funcionalidade 5 que busca por registros de data 'data'
	void busca_data (FILE* binario, char* data) {

		char removido; int tam;
		HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v;

		readHeaderBin(h, binario);
		if(h->status == '0'){
			printf("Falha no processamento do arquivo.\n");
			return;
		}

		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}

		while (check(binario)) {
			
			v = create();
			leituraBinario(binario, v, &removido, &tam);
			
			if (!removido) {
				libera(v);
				continue;
			}

			if (strcmp(v->data, data) == 0) imprimeVeiculo(v);

			libera(v);

		}
		liberaHeader(h);
	}

	// Função auxiliar da funcionalidade 5 que busca por registros com quantidade de lugares 'qtdlug'
	void busca_qtdlug (FILE* binario, int qtdlug) {

		char removido; 
		int tam;
		HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v;

		readHeaderBin(h, binario);
		if(h->status == '0'){
			printf("Falha no processamento do arquivo.\n");
			return;
		}

		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}
		
		while (check(binario)) {
			
			v = create();
			leituraBinario(binario, v, &removido, &tam);
			
			if (!removido) {
				libera(v);
				continue;
			}


			if (v->quatidadeLugares == qtdlug) imprimeVeiculo(v);

			libera(v);
		}
		liberaHeader(h);
	}

	// Função auxiliar da funcionalidade 5 que busca por registros de modelo 'modelo'
	void busca_modelo (FILE* binario, char* modelo) {

		char removido; int tam;
		HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v;

		readHeaderBin(h, binario);
		
		if(h->status == '0'){
			printf("Falha no processamento do arquivo.\n");
			return;
		}

		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}

		while (check(binario)) {
			
			v = create();
			leituraBinario(binario, v, &removido, &tam);
			
			if (!removido) {
				libera(v);
				continue;
			}

			if (strcmp(v->modelo, modelo) == 0) imprimeVeiculo(v);

			libera(v);
		}
		liberaHeader(h);
	}

	// Função auxiliar da funcionalidade 5 que busca por registros de categoria 'categoria'
	void busca_categoria (FILE* binario, char* categoria) {

		char removido; int tam;
		HeaderVeiculo* h = criarHeader(); RegistroVeiculo* v;

		readHeaderBin(h, binario);
		
		if(h->status == '0'){
			printf("Falha no processamento do arquivo.\n");
			return;
		}

		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			return;
		}

		while (check(binario)) {
			
			v = create();
			
			leituraBinario(binario, v, &removido, &tam);
			
			if (!removido) {
				libera(v);
				continue;
			}

			if (strcmp(v->categoria, categoria) == 0) imprimeVeiculo(v);

			libera(v);
		}
		liberaHeader(h);
	}

//Funcionalidades
	// Funcionalidade 1: Transferencia de dados do arquivo .csv para o binario
	void func1(FILE* fp, FILE* binario) {

		HeaderVeiculo* h = criarHeader();
		
		descreveHeader(h, fp);
		setHeader(h, binario);
		
		// Enquanto não chegamos no fim do arquivo
		while (check(fp)) {
			
			RegistroVeiculo* v = create();
			leitura(fp, v);
			atualizaBinario(h, v, binario);
			libera(v);

		}

		atualizaHeader(h, binario);
		liberaHeader(h);
	}


	// Funcionalidade 3: Imprimir os registros de um arquivo binario
	// OBS: A função deveria ser mais compacta, já que as funções anteriores 'leituraBinario'
	// e 'imprimeVeiculo' poderiam ser usadas para modularizar o codigo. Porém, não conseguimos
	// achar a causa do bug que acontece quando usamos as funções, por isso deixamos o código assim.
	void func3(FILE* binario) {

		char removido, prefixo[6], data[11], *modelo, *categoria;
		int tam, qtdlug, tamodelo, tamcategoria, codlinha;
		HeaderVeiculo* h = criarHeader();

		readHeaderBin(h, binario);
		if(h->status == '0'){
			printf("Falha no processamento do arquivo.\n");
			liberaHeader(h);
			return;
		}

		if(h->nroRegistros == 0){
			printf("Registro inexistente.\n");
			liberaHeader(h);
			return;
		}

		// Enquanto não chegamos no fim do arquivo
		while (check(binario)) {
			fread(&(removido), sizeof(char), 1, binario);
			
			if (removido == '0') {
				fread(&(tam), sizeof(int), 1, binario);
				fseek(binario, tam, SEEK_CUR);
				continue;
				
			}  

			fread(&(tam), sizeof(int), 1, binario);

			fread(prefixo, sizeof(char), 5, binario);
			prefixo[5] = '\0';

			fread(data, sizeof(char), 10, binario);
			data[10] = '\0';

			fread(&(qtdlug), sizeof(int), 1, binario);

			fread(&(codlinha), sizeof(int), 1, binario);

			fread(&(tamodelo), sizeof(int), 1, binario);
			if (tamodelo != 0) {
				
				modelo = (char*)malloc(sizeof(char) * tamodelo + 1); 
				fread(modelo, sizeof(char), tamodelo, binario);
				modelo[tamodelo] = '\0';
			
			}

			fread(&(tamcategoria), sizeof(int), 1, binario);
			if (tamcategoria != 0) {
				
				categoria = (char*)malloc(sizeof(char) * tamcategoria + 1); 
				fread(categoria, sizeof(char), tamcategoria, binario);
				categoria[tamcategoria] = '\0';
			
			}
			

			printf("Prefixo do veiculo: %s\n", prefixo);

			printf("Modelo do veiculo: ");
			if (tamodelo) printf("%s\n", modelo);
			else printf("campo com valor nulo\n");

			if (tamcategoria) printf("Categoria do veiculo: %s\n", categoria);
			else printf("campo com valor nulo\n");

			printf("Data de entrada do veiculo na frota: ");
			if (data[1] == '@') printf("campo com valor nulo\n");
			else printData(data);

			if (qtdlug != -1) printf("Quantidade de lugares sentados disponiveis: %d\n", qtdlug);
			else printf("campo com valor nulo\n");
			
			printf("\n");

			if (tamodelo) free(modelo);
			if (tamcategoria) free(categoria);

		}
		liberaHeader(h);
	}


	// Funcionalidade 5: Busca por registros com campo 'nomeDoCampo' de valor 'valor'
	void func5(FILE* binario, char* nomeDoCampo, char* valor) {

		if (strcmp("prefixo", nomeDoCampo) == 0) {
			busca_prefixo(binario, valor);
		}
		else if (strcmp("quantidadeLugares", nomeDoCampo) == 0) {
			
			int qtd;
			
			if (strcmp(valor, "NULO") == 0) qtd = -1;
			else qtd = atoi(valor);
			
			busca_qtdlug(binario, qtd);
		
		}
		else if (strcmp("modelo", nomeDoCampo) == 0) {
			busca_modelo(binario, valor);
		}
		else if (strcmp("data", nomeDoCampo) == 0) {
			busca_data(binario, valor);
		}
		else if (strcmp("categoria", nomeDoCampo) == 0) {
			busca_categoria(binario, valor);
		}

	}

	// Funcionalidade 7: Inserir 'n' registros em um arquivo binário 
	int func7(FILE* binario, int n) {

		HeaderVeiculo* h = criarHeader();

		readHeaderBin(h, binario);
		
		if(h->status == '0'){
			printf("Falha no processamento do arquivo.\n");
			liberaHeader(h);
			return 0;
		}

		char prefixo[7];
		char data[12];
		int lug = 0;
		char lugNaoTratado[4];
		char codNaoTratado[6];
		int codl = 0;
		char modelo[100];
		char categoria[100];
		int i = 0;

		while (i < n) { 
			
			RegistroVeiculo* v = create();
			
			scan_quote_string(prefixo);
			
			scan_quote_string(data);
			
			if(strcmp(data, "") == 0){
				strcpy(data, "NULO");
			}
			
			scanf("%s %s", lugNaoTratado, codNaoTratado);
			
			if((lugNaoTratado[0] == 'N') || (lugNaoTratado[0] == 'n')){
				lug = -1;
			}else{
				lug = atoi(lugNaoTratado);
			}

			if((codNaoTratado[0] == 'N') || (codNaoTratado[0] == 'n')){
				codl = -1;
			}else{
				codl = atoi(codNaoTratado);
			}
			
			scan_quote_string(modelo);
			
			if(strcmp(modelo, "") == 0){
				strcpy(modelo, "NULO");
			}
			
			scan_quote_string(categoria);
			
			if(strcmp(categoria, "") == 0){
				strcpy(categoria, "NULO");
			}
			
			setVeiculo(v, prefixo, data, lug, codl, modelo, categoria);
			atualizaBinario(h, v, binario);

			libera(v);
			
			i++;
		
		}

		atualizaHeader(h, binario);

<<<<<<< HEAD
	liberaHeader(h);
	
	return 1;
}
=======
		liberaHeader(h);
		
		return 1;
	}
>>>>>>> master
