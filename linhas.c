// Cabeçalho

	struct cabecalhoLinha
	{
		// Tamanho fixo
		// Total: 82 Bytes
		char 	 status; 				//  1 byte
		long int byteProxReg;			//  8 bytes
		int 	 nroRegistros; 			//  4 bytes
		int 	 nroRegistrosRemovidos;	//  4 bytes
		char 	 descreveCodigo[16];	// 15 bytes
		char 	 descreveCartao[14];	// 13 bytes
		char 	 descreveNome[14];		// 13 bytes
		char 	 descreveCor[25]; 		// 24 bytes
	};

	HeaderLinha* criarHeaderLinha(){
		HeaderLinha* h = (HeaderLinha*)malloc(sizeof(HeaderLinha));
		h->status = '0';
		h->byteProxReg = 82;
		h->nroRegistros = 0;
		h->nroRegistrosRemovidos = 0;

		if(DEBUG) printDebugCabecalhoLinha(h);

		return h;
	}

	void leituraCabecalhoLinhaCsv(HeaderLinha* h, FILE* fp){
		HeaderLinha* h = criarHeaderLinha();

		fscanf(fp, "%[^,],", h->descreveCodigo);
		fscanf(fp, "%[^,],", h->descreveCartao);
		fscanf(fp, "%[^,],", h->descreveNome);
		fscanf(fp, "%[^\n]\n", h->descreveCor);

		if(DEBUG) printDebugCabecalhoLinha(h);
	}

	HeaderLinha* leituraCabecalhoLinhaBinario(FILE* fp){
		HeaderLinha* h = (HeaderLinha*)malloc(sizeof(HeaderLinha));

		fread(h->status, 				sizeof(char),	  1, fp);
		fread(h->byteProxReg, 			sizeof(long int), 1, fp);
		fread(h->nroRegistros, 			sizeof(int), 	  1, fp);
		fread(h->nroRegistrosRemovidos, sizeof(int), 	  1, fp);
		fread(h->descreveCodigo, 		sizeof(char), 	 16, fp);
		fread(h->descreveCartao, 		sizeof(char), 	 14, fp);
		fread(h->descreveNome,			sizeof(char), 	 14, fp);
		fread(h->descreveCor, 			sizeof(char), 	 25, fp);

		if(DEBUG) printDebugCabecalhoLinha(h);

		return h;
	}

	void escritaCabecalhoLinha(HeaderLinha* h, FILE* fp){
		fwrite(h->status, 				 sizeof(char),     1,  fp);
		fwrite(h->byteProxReg, 			 sizeof(long int), 1,  fp);
		fwrite(h->nroRegistros,			 sizeof(int),      1,  fp);
		fwrite(h->nroRegistrosRemovidos, sizeof(int),      1,  fp);
		fwrite(h->descreveCodigo,		 sizeof(char),    15,  fp);
		fwrite(h->descreveCartao,		 sizeof(char),    13,  fp);
		fwrite(h->descreveNome,			 sizeof(char),    13,  fp);
		fwrite(h->descreveCor,			 sizeof(char),    24,  fp);

		if(DEBUG) printDebugCabecalhoLinha(h);
	}

	void atualizaCabecalhoLinha(HeaderLinha*h, FILE* fp){
		fseek(fp, 0, SEEK_SET);
		h->status = '1';
		fwrite(h->status, 				 sizeof(char),      1,  fp);
		fwrite(h->byteProxReg, 			 sizeof(long int),  1,  fp);
		fwrite(h->nroRegistros,			 sizeof(int),       1,  fp);
		fwrite(h->nroRegistrosRemovidos, sizeof(int),       1,  fp);

		if(DEBUG) printDebugCabecalhoLinha(h);
	}

	void liberaCabecalhoLinha(HeaderLinha* h){
		if(h != NULL) free(h);
	}


	// DEBUG PRINT
	void printDebugCabecalhoLinha(HeaderLinha* h){
		if(h == NULL){
			printf("Nao existe header ainda\n"); 
			return;
		} 
		printf("Prints das descricoes cabecalho linha\n");
		printf("%s\n", h->descreveCodigo);
		printf("%s\n", h->descreveCartao);
		printf("%s\n", h->descreveNome);
		printf("%s\n", h->descreveCor);
	}

	///////////////////////////////////////////////////////////////////////
// Registros
	
	RegistroLinha* criaRegistroLinha(void){
		RegistroLinha *reg = (RegistroLinha*)malloc(sizeof(RegistroLinha));
		return reg;
	}
	
	int leRegistroLinhaCsv(RegistroLinha* reg, FILE* fp){
		// Codigo da linha,
		// Aceita cartao,
		// Nome da linha, 
		// Cor que descreve a linha

		// ver se precisa collocar 4 pra ler o \0

		char check;
		if(fread(&check, sizeof(char), 1, fp) != 1){
			return 0;
		}

		fseek(ponteiro, -sizeof(char), SEEK_CUR);
		char codigoLinhaNaoTratado[3];

		fscanf(fp, "%[^,],",  codigoLinhaNaoTratado);

		if(codigoLinhaNaoTratado[0] != '*'){
			reg->codLinha = atoi(codigoLinhaNaoTratado);
			reg->removido = '1';
		}else{
			codigoLinhaNaoTratado[0] = codigoLinhaNaoTratado[1];
			codigoLinhaNaoTratado[1] = codigoLinhaNaoTratado[2];
			codigoLinhaNaoTratado[2] = '\0';
			reg->removido = '0';
			// ver se funciona esse atoi
			reg->codLinha = atoi(codigoLinhaNaoTratado);
		}

		char* aceitaCartaoNaoTratado;
		char* nomeLinhaNaoTratado;
		char* corLinhaNaoTratado;
		char  nulo[] = 'NULO'

		fscanf(fp, "%m[^,],", aceitaCartaoNaoTratado);
		fscanf(fp, "%m[^,],", nomeLinhaNaoTratado);
		fscanf(fp, "%m[^,],", corLinhaNaoTratado);

		// Identificação de valores nulos 
		if(strcmp(aceitaCartaoNaoTratado, nulo)){
			reg->aceitaCartao = '\0';

		}else{
			strcpy(reg->aceitaCartao, aceitaCartaoNaoTratado);
		}		

		if(strcmp(nomeLinhaNaoTratado, nulo)){
			reg->tamanhoNome = 0;
		}else{
			strcpy(reg->nomeLinha, nomeLinhaNaoTratado);
			reg->tamanhoNome = sizeof(reg->nomeLinha);
		}

		if(strcmp(corLinhaNaoTratado, nulo)){
			reg->tamanhoCor = 0;
		}else{
			strcpy(reg->corLinha, nomeLinhaNaoTratado);
			reg->tamanhoCor = sizeof(reg->corLinha);
		}

		reg->tamanhoRegistro = 18 + reg->tamanhoCor + reg->tamanhoNome;

		return 1;	
	}

	void liberaRegistroLinha(RegistroLinha* reg){
		if(reg != NULL){
			if(reg->nomeLinha != NULL) free(reg->);
			if(reg->corLinha != NULL) free(reg->);
			free(reg);
		}
	}

	void atualizaCabecalhoBinarioLinha(HeaderLinha* h, RegistroLinha* reg, FILE* fp){
		int  tam;

		if(reg->removido = '1') {
			h->nroRegistrosRemovidos++;
		}else{
			h->nroRegistros++;
		}

		tam = escreveRegistroLinha(reg, removido, h->byteProxReg, fp);
		h->byteProxReg = tam + h->byteProxReg;

		fseek(fp, 1, SEEK_SET);
		fwrite(&(h->byteProxReg), 			sizeof(long int), 1, fp);
		fwrite(&(h->nroRegistros), 			sizeof(int),      1, fp);
		fwrite(&(h->nroRegistrosRemovidos), sizeof(int), 	  1, fp);
	}

	void imprimeRegistroLinha(RegistroLinha* reg){
		/*
			Nome da linha: campo com valor nulo
			Cor que descreve a linha: AMARELA
			Aceita cartao: PAGAMENTO EM CARTAO SEM COBRADOR
			--- pule uma linha em branco ---
		*/

		if(reg->removido = '1'){
			printf("Registro inexistente.\n");
			return;
		}
		printf("Nome da linha: \n");
		if(reg->tamanhoNome = 0){
			printf("campo com valor nulo\n");
		}else{
			printf("%s\n", reg->nomeLinha);
		}
		printf("Cor que descreve a linha: \n");
		if(reg->tamanhoCor = 0){
			printf("campo com valor nulo\n");
		}else{
			printf("%s\n", reg->corLinha);
		}
		printf("Aceita cartao: \n");
		switch(reg->aceitaCartao){
			case 'S':
				printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
				break;
			case 'N':
				printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
				break;
			case 'F':
				printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
				break;
			case '\0':
				printf("campo com valor nulo\n");
				break;
		}
		printf("\n");
	}

	int escreveRegistroLinha(RegistroLinha* reg, char removido, int byteProxReg, FILE* fp){
		// Retorna o tamanho total do registro

		// Setta o ponteiro na posicao que vai escrever
		fseek(fp, byteProxReg, SEEK_SET);

		// Escrita dos dados já formatados (tamanhos fixos)
		fwrite(&(reg->removido), sizeof(char), 1, fp);
		fwrite(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
		fwrite(&(reg->codLinha), sizeof(int),  1, fp);
		fwrite(&(reg->aceitaCartao), sizeof(char), 0, fp);

		// escrita dos dados de tamanho variável
		fwrite(&(reg->tamanhoNome), sizeof(int),  1, fp);
		if(reg->tamanhoNome != 0){
			// nomelinha
			fwrite(&(reg->nomeLinha), sizeof(char), reg->tamanhoNome, fp);
		}
		fwrite(&(reg->tamanhoCor), sizeof(int),  1, fp);
		if(reg->tamanhoCor != 0){
			// corlinha
			fwrite(&(reg->corLinha), sizeof(char), reg->tamanhoCor, fp);
		}

		return reg->tamanhoRegistro;
	}

	struct registroLinha
	{
		// Registro de tamanho variável
		char  removido; 		// 1 byte
		int   tamanhoRegistro;  // 4 bytes

		// Não pode ser nulo
		int   codLinha; 		// 4 bytes
		char  aceitaCartao;		// 1 byte
		
		int   tamanhoNome; 		// 4 bytes
		char* nomeLinha;		// variavel

		int   tamanhoCor;		// 4 bytes
		char* corLinha; 		// variavel
	};

	// AINDA NÃO TERMINEI A PARTIR DAQUI
	void leRegistroLinha(RegistroLinha* reg, FILE* fp){

		// Leitura dos dados já formatados (tamanhos fixos)
		fread(&(reg->removido), sizeof(char), 1, fp);
		fread(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
		fread(&(reg->codLinha), sizeof(int),  1, fp);
		fread(&(reg->aceitaCartao), sizeof(char), 0, fp);

		// Leitura dos dados de tamanho variável
		fread(&(reg->tamanhoNome), sizeof(int),  1, fp);
		if(reg->tamanhoNome != 0){
			// nomelinha
			fread(&(reg->nomeLinha), sizeof(char), reg->tamanhoNome, fp);
		}
		fread(&(reg->tamanhoCor), sizeof(int),  1, fp);
		if(reg->tamanhoCor != 0){
			// corlinha
			fread(&(reg->corLinha), sizeof(char), reg->tamanhoCor, fp);
		}

		return 1;
	}

	void buscaRegistroLinha(FILE* fp){
		RegistroLinha* reg = criaRegistroLinha();
		HeaderLinha* h = leituraCabecalhoLinhaCsv(fp);

		// Ler cada registro, procurar o valor descrito no nome do campo
		// printar esse registro com imprimeRegistroLinha()
	}

	//////////////////////////////////////////////////////////////////////////
	///// Escrita e busca tão copiados do veiculos pra referencia rapida /////
	//////////////////////////////////////////////////////////////////////////


	// DEBUG PRINT
	void printDebugRegistroLinha(RegistroLinha* reg){
		if(reg == NULL){
			printf("Não existe esse registro\n");
			return;
		}
		printf("%s\n", reg->removido);
		printf("%d\n", reg->tamanhoRegistro);
		printf("%d\n", reg->codlinha);
		printf("%s\n", reg->aceitaCartao);
		printf("%d\n", reg->tamanhoNome);
		printf("%s\n", reg->nomeLinha);
		printf("%d\n", reg->tamanhoCor);
		printf("%s\n", reg->corLinha);
	}

// Funcionalidades
	void funcionalidade2(FILE* fpCsv, FILE* fpBin){
		// Escrita dos registros num binario

		// Cria as structs
		HeaderLinha* h  = criarHeaderLinha();
		RegistroLinha* reg = criaRegistroLinha();

		// Le e escreve o cabecalho
		leituraCabecalhoLinhaCsv(h, fpCsv);
		escritaCabecalhoLinha(h, fpBin);

		// Le e escreve cada registro
		while(leRegistroLinhaCsv(reg, fpCsv)){
			atualizaCabecalhoBinarioLinha(h, reg, fpBin);
			free(reg);
			reg = criaRegistroLinha();
		}
		free(reg);
	}

	void funcionalidade4(FILE* fpBin){

	}

	void funcionalidade6(FILE* fpBin, char nomeDoCampo[17], char valor[100]){

	}

	void funcionalidade8(FILE* fpBin, int qtdRegistros){

	}
