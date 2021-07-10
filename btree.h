#define ERROR -1 		// valor de retorno da função pageInsert quando já existe um elemento a ser inserido
#define PROMOTION 1 	// valor de retorno da função pageInsert quando uma chave é promovida
#define NOPROMOTION 0 	// valor de retorno da função pageInsert quando não há promoção

typedef struct PAGE page;
void writeHeader(FILE* bin, char status, int rootRRN, int proxRRN);
page* createPage();
int hasChilds(page* pag);
void resetPage(page* pag);
int getRRN(page* p);
void writePage(FILE* bin, page* pag);
void readPage(FILE* bin, page* pag);
void printPage(page* pag);
void split(page* pag, int* promo_key, long int* promo_PR, int* promo_r_child, page* newpage);
int pageInsert(FILE* bin, int current_rrn, int key, long int PR, int* promo_r_child, int* promo_key, long int* promo_PR, int* last_rrn);
void btreeInsert(FILE* bin, page** root, int key, long int PR, int* RRN);
int searchKey(FILE* bin, int current_rrn, int key);
void printBTree(FILE* bin, int current_rrn);