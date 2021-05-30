#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linhas.h"
#include "veiculo.h"
#include "auxiliares.h"

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
			func1(fpCsv, fpBin);

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
			if(DEBUG) printf("Fechou os arquivos\n");
			binarioNaTela(nomeArquivoBinario);
			if(DEBUG) printf("Saiu do switch\n");
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

			func3(fpBin);

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

			//printf("%s, %s", nomeDoCampo, valor);
			// Leitura do veiculo.bin
			// Busca por algum criterio
			// Escrita formatada dos reg que satisfazem
			func5(fpBin, nomeDoCampo, valor);

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
			func7(fpBin, qtdRegistros);
			
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
