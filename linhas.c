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

	void criarHeaderLinha(){
		HeaderLinha* h = (HeaderLinha*)malloc(sizeof(HeaderLinha));
		h->status = '0';
		h->byteProxReg = 0;
		h->nroRegistros = 0;
		h->nroRegistrosRemovidos = 0;
	}


	void leituraCabecalhoLinhaCsv(HeaderLinha* h, FILE* fp){
		fscanf(fp, "%[^,],", h->descreveCodigo);
		fscanf(fp, "%[^,],", h->descreveCartao);
		fscanf(fp, "%[^,],", h->descreveNome);
		fscanf(fp, "%[^\n]\n", h->descreveCor);

		if(DEBUG) printDebugCabecalhoLinha(h);
	}

	void escritaCabecalhoLinha(HeaderLinha* h, FILE* fp){
		fwrite(h->status, 				 sizeof(char),     1,  fp);
		fwrite(h->byteProxReg, 			 sizeof(long int), 1,  fp);
		fwrite(h->nroRegistros,			 sizeof(int),      1,  fp);
		fwrite(h->nroRegistrosRemovidos, sizeof(int),      1,  fp);
		fwrite(h->descreveCodigo,		 sizeof(char),     15, fp);
		fwrite(h->descreveCartao,		 sizeof(char),     13, fp);
		fwrite(h->descreveNome,			 sizeof(char),     13, fp);
		fwrite(h->descreveCor,			 sizeof(char),     24, fp);
		h->byteProxReg = 82; // Ou 83????

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
			reg->nomeLinha = '\0';
		}else{
			strcpy(reg->nomeLinha, nomeLinhaNaoTratado);
		}

		if(strcmp(corLinhaNaoTratado, nulo)){
			reg->corLinha = '\0';
		}else{
			strcpy(reg->corLinha, nomeLinhaNaoTratado);
		}


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

	// AINDA NÃO TERMINEI A PARTIR DAQUI

	int escreveRegistroLinha(RegistroLinha* reg, char removido, int byteProxReg, FILE* fp){
		// Retorna o tamanho total do registro
		// TODO

		// removido
		fwrite(&(removido), sizeof(char), 1, fp);


		// tamanho registro
		fwrite(&(removido), sizeof(int),  0, fp);
		// codlinha
		fwrite(&(removido), sizeof(int),  0, fp);
		// aceita cartao
		fwrite(&(removido), sizeof(char), 0, fp);
		// tamanho nome
		fwrite(&(removido), sizeof(int),  0, fp);
		// nomelinha
		fwrite(&(removido), sizeof(char), 0, fp);
		// tamanho cor
		fwrite(&(removido), sizeof(int),  0, fp);
		// corlinha
		fwrite(&(removido), sizeof(char), 0, fp);
	}

	void buscaRegistroLinha(){
		
	}

	//////////////////////////////////////////////////////////////////////////
	///// Escrita e busca tão copiados do veiculos pra referencia rapida /////
	//////////////////////////////////////////////////////////////////////////

	int escrita (veiculo* v, char removido, int byteProxReg, FILE* binario) {

		int tamodelo, flag = 0;
		int tamcategoria = strlen(v->categoria);
		int tam = 31 + tamcategoria;

		if (strcmp(v->modelo, "NULO") == 0) {
			flag = 1;
		} 
		else {
			tamodelo = strlen(v->modelo);
			tam += tamodelo;
		}

		fseek(binario, byteProxReg, SEEK_SET);

		fwrite(&(removido), sizeof(char), 1, binario);
		fwrite(&(tam), sizeof(int), 1, binario);
		fwrite(v->prefixo, sizeof(char), strlen(v->prefixo), binario);

		if (v->data[0] == 'N') {

			char lixo = '@';
			for (int i = 0; i < 10; i++) {
				fwrite(&lixo, sizeof(char), 1, binario); 
			}

		} else {
			fwrite(v->data, sizeof(char), strlen(v->data), binario); 
		}
		
		fwrite(&(v->quatidadeLugares), sizeof(int), 1, binario);
		
		
		if (v->codLinha == 0){

			int lixo = -1;
			fwrite(&lixo, sizeof(int), 1, binario);

		} else {
			fwrite(&(v->codLinha), sizeof(int), 1, binario); 
		}
		
		
		if (flag == 1) {

			int tam = 0;
			fwrite(&tam, sizeof(int), 1, binario);

		} else {

			fwrite(&(tamodelo), sizeof(int), 1, binario);  
			fwrite(v->modelo, sizeof(char), tamodelo, binario); 

		}
		
		
		fwrite(&(tamcategoria), sizeof(int), 1, binario);
		fwrite(v->categoria, sizeof(char), tamcategoria, binario);
		int total = tam + 5;

		return total;
	}

	void busca (FILE* binario, char* campo, int n) {

		/*	Prefixo do veiculo: BA004
			Modelo do veiculo: NEOBUS MEGA
			Categoria do veiculo: ALIMENTADOR
			Data de entrada do veiculo na frota: 29 de maio de 2009
			Quantidade de lugares sentados disponiveis: campo com valor nulo
			--- pule uma linha em branco ---
		*/

		char removido, prefixo[6], data[11], *modelo, *categoria;
		int tam, qtdlug, tamodelo, tamcategoria, codlinha, i = 0, fdata = 0;
		fseek(binario, 175, SEEK_SET);

		while (i < n) {
			
			fread(&(removido), sizeof(char), 1, binario);
			if (removido == '0'){
				i++;
				printf("Registro inexistente\n");
				fread(&(tam), sizeof(int), 1, binario);
				fseek(binario, tam, SEEK_CUR);
				continue;
			}

			fread(&(tam), sizeof(int), 1, binario);
			//printf("Tamanho do registro: %d\n", tam);

			fread(prefixo, sizeof(char), 5, binario);
			prefixo[5] = '\0';
			//printf("Prefixo do veiculo: %s\n", prefixo);

			fread(data, sizeof(char), 10, binario);
			data[10] = '\0';
			if (data[0] == '@') fdata = 1;
			//printf("Data de entrada do veiculo na frota: %s\n", data);

			fread(&(qtdlug), sizeof(int), 1, binario);
			//printf("Quantidade de lugares sentados disponiveis: %d\n", qtdlug);

			fread(&(codlinha), sizeof(int), 1, binario);
			//printf("Codigo da linha: %d\n", codlinha);

			fread(&(tamodelo), sizeof(int), 1, binario);
			if (tamodelo != 0) {
				
				modelo = (char*)malloc(sizeof(char) * tamodelo + 1); 
				fread(modelo, sizeof(char), tamodelo, binario);
				modelo[tamodelo] = '\0';
			
			}
			//printf("Modelo do veiculo: %s\n", modelo);

			fread(&(tamcategoria), sizeof(int), 1, binario);
			categoria = (char*)malloc(sizeof(char) * tamcategoria + 1); 
			fread(categoria, sizeof(char), tamcategoria, binario);
			categoria[tamcategoria] = '\0';
			//printf("Categoria do veiculo: %s\n", categoria);

			if (strcmp(campo, prefixo) == 0) {

				printf("Tamanho do registro: %d\n", tam);
				printf("Prefixo do veiculo: %s\n", prefixo);
				printf("Data de entrada do veiculo na frota: ");
				if (fdata) printf("NULO\n");
				else printf("%s\n", data);

				printf("Quantidade de lugares sentados disponiveis: %d\n", qtdlug);
				printf("Codigo da linha: %d\n", codlinha);

				printf("Modelo do veiculo: ");
				if (tamodelo) printf("%s\n", modelo);
				else printf("NULO\n");
				printf("Categoria do veiculo: %s\n", categoria);
				printf("\n");

			}

			if (tamodelo) free(modelo);
			free(categoria);

			i++;

		}
	}



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