#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"

int main(int argc, char const *argv[])
{
	int opNum;
	char nomeArquivoCsv[20];
	char nomeArquivoBinario[20];
	char nomeArquivoIndex[20];
	FILE* fpCsv;
	FILE* fpBin;
	FILE* fpIndex;
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
			if(fpCsv == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			fpBin = fopen(nomeArquivoBinario, "wb+");
			if(fpBin == NULL){
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
			if(fpCsv == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			fpBin = fopen(nomeArquivoBinario, "wb+");
			if(fpBin == NULL){
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

			fpBin = fopen(nomeArquivoBinario, "rb+");
			if(fpBin == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			// Entrada: registro de veiculo
			// Insercao no veiculo.bin
			if(func7(fpBin, qtdRegistros)){
				fclose(fpBin);
				binarioNaTela(nomeArquivoBinario);
			}else{
				fclose(fpBin);
			}
			
			// Binario na tela

			break;
		case 8:
			scanf("%s %d", nomeArquivoBinario, &qtdRegistros);

			fpBin = fopen(nomeArquivoBinario, "rb+");
			if(fpBin == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			// ENtrada: registro de linha
			// Insercao no linha.bin
			if(funcionalidade8(fpBin, qtdRegistros)){
				fclose(fpBin);
				binarioNaTela(nomeArquivoBinario);
			}else{
				fclose(fpBin);
			}
			// binario na tela

			break;
		case 9:
		case 10:
			scanf("%s %s", nomeArquivoBinario, nomeArquivoIndex);

			fpBin = fopen(nomeArquivoBinario, "rb");
			fpIndex = fopen(nomeArquivoIndex, "wb+");

			if((fpIndex == NULL) || (fpBin == NULL)){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			funcionalidade10(nomeArquivoBinario, nomeArquivoIndex);
			fclose(fpBin);
			fclose(fpIndex);
			binarioNaTela(nomeArquivoIndex);
			break;
		case 11:
			break;
		case 12:
			scanf("%s %s", nomeArquivoBinario, nomeArquivoIndex);
			fpBin = fopen(nomeArquivoBinario, "rb");
			fpIndex = fopen(nomeArquivoIndex, "wb+");

			if((fpIndex == NULL) || (fpBin == NULL)){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			funcionalidade12(nomeArquivoBinario, nomeArquivoIndex);
			fclose(fpBin);
			fclose(fpIndex);
			break;
		case 13:
			break;
		case 14:
			scanf("%s %s %d", nomeArquivoBinario, nomeArquivoIndex, &qtdRegistros);
			fpBin = fopen(nomeArquivoBinario, "rb");
			fpIndex = fopen(nomeArquivoIndex, "wb+");

			if((fpIndex == NULL) || (fpBin == NULL)){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			funcionalidade14(nomeArquivoBinario, nomeArquivoIndex, qtdRegistros);
			fclose(fpBin);
			fclose(fpIndex);
			binarioNaTela(nomeArquivoIndex);
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
