struct cabecalhoLinha
{
	// Tamanho fixo
	// Total: 82 Bytes
	char status; 			//  1 byte
	int  byteProxReg;		//  8 bytes
	int  nroRegistros; 		//  4 bytes
	int  nroRegRemovidos;	//  4 bytes
	char descreveCodigo[15];// 15 bytes
	char descreveCartao[13];// 13 bytes
	char descreveNome[13];	// 13 bytes
	char descreveCor[24]; 	// 24 bytes
};

struct registroLinha
{
	// Registro de tamanho variável
	char  removido; 		// 1 byte
	int   tamanhoRegistro;  // 4 bytes
	int   codLinha; 		// 4 bytes
	char  aceitaCartao;		// 1 byte
	
	int   tamanhoNome; 		// 4 bytes
	char* nomeLinha;		// variavel

	int   tamanhoCor;		// 4 bytes
	char* corLinha; 		// variavel

};

