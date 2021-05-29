#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "linhas.h"
#include "veiculos.h"


#define DEBUG 1

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

int main(int argc, char const *argv[])
{
	int opNum;
	char nomeArquivoCsv[20];
	char nomeArquivoBinario[20];
	FILE* fpCsv;
	FILE* fpBin;
	int qtdRegistros;
	char nomeDoCampo[17];
	char valor[100];

	//printf("Qual operacao?\n");
	scanf("%d", &opNum);

	switch(opNum){
		case 1:
			// Leitura do veiculo.csv
			scanf("%s %s", nomeArquivoCsv, nomeArquivoBinario);
			fpCsv = fopen(nomeArquivoCsv, "r");
			fpBin = fopen(nomeArquivoBinario, "wb+");
			if((fpCsv == NULL) || (fpBin == NULL)){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}

			// Escrita dos registros num binario
			//funcionalidade1(fpCsv, fpBin);

			// binario na tela
			fclose(fpCsv);
			fclose(fpBin);
			binarioNaTela(nomeArquivoBinario);
			break;
		case 2:
			// Leitura de linha.csv
			scanf("%s %s", nomeArquivoCsv, nomeArquivoBinario);
			fpCsv = fopen(nomeArquivoCsv, "r");
			fpBin = fopen(nomeArquivoBinario, "wb+");
			if((fpCsv == NULL) || (fpBin == NULL)){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}

			funcionalidade2(fpCsv, fpBin);
			// Escrita dos registros num binario
			// binario na tela
			fclose(fpCsv);
			fclose(fpBin);
			binarioNaTela(nomeArquivoBinario);
			break;
		case 3:
			scanf("%s", nomeArquivoBinario);

			fpBin = fopen(nomeArquivoBinario, "rb");

			if(fpBin == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			// Leitura do veiculo.bin
			// Escrita formatada de cada registro

			//funcionalidade3(nomeArquivoBinario);

			fclose(fpBin);
			break;
		case 4:
			scanf("%s", nomeArquivoBinario);

			fpBin = fopen(nomeArquivoBinario, "rb");
			if(fpBin == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}

			// Leitura do veiculo.bin
			// Escrita formatada de cada registro
			funcionalidade4(fpBin);
			fclose(fpBin);
			break;
		case 5:
			scanf("%s %s", nomeArquivoBinario, nomeDoCampo);
			scan_quote_string(valor);

			fpBin = fopen(nomeArquivoBinario, "rb");
			if(fpBin == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			// Leitura do veiculo.bin
			// Busca por algum criterio
			// Escrita formatada dos reg que satisfazem
			//funcionalidade5(fpBin, nomeDoCampo, valor);

			fclose(fpBin);
			break;
		case 6:
			scanf("%s %s", nomeArquivoBinario, nomeDoCampo);
			scan_quote_string(valor);

			fpBin = fopen(nomeArquivoBinario, "rb");
			if(fpBin == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			// Leitura do linha.bin
			// Busca por algum criterio
			// Escrita formatada dos reg que satisfazem
			funcionalidade6(fpBin, nomeDoCampo, valor);

			fclose(fpBin);
			break;
		case 7:
			scanf("%s %d", nomeArquivoBinario, &qtdRegistros);

			fpBin = fopen(nomeArquivoBinario, "wb+");
			if(fpBin == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			// Entrada: registro de veiculo
			// Insercao no veiculo.bin
			//funcionalidade7(fpBin, qtdRegistros);
			
			// Binario na tela
			fclose(fpBin);
			binarioNaTela(nomeArquivoBinario);

			break;
		case 8:
			scanf("%s %d", nomeArquivoBinario, &qtdRegistros);

			fpBin = fopen(nomeArquivoBinario, "wb+");
			if(fpBin == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			// ENtrada: registro de linha
			// Insercao no linha.bin
			funcionalidade8(fpBin, qtdRegistros);

			// binario na tela
			fclose(fpBin);
			binarioNaTela(nomeArquivoBinario);

			break;
		default:
			// Erro de entrada
			break;
	}
	return 0;
}




/* ---------------- EXTRA ----------------

OPCIONAL: dicas sobre scanf() e fscanf():

scanf("%[^,]", string) -> lê até encontrar o caractere ',', não incluindo o mesmo na leitura.

Exemplo de entrada: "Oi, esse é um exemplo."
Nesse caso, o scanf("%[^,]") tem como resultado a string "Oi";

scanf("%[^\"]", string) -> lê até encontrar o caractere '"', não incluindo o mesmo na leitura.
scanf("%[^\n]", string) -> lê até encontrar o fim da linha, não incluindo o '\n' na leitura.

scanf("%*c") --> lê um char e não guarda em nenhuma variável, como se tivesse ignorado ele

*/
