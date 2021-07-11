#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H


void func1(FILE* fp, FILE* binario);
void func3(FILE* binario);
void func5(FILE* binario, char* nomeDoCampo, char* valor);
int func7(FILE* binario, int n);

void funcionalidade2(FILE* fpCsv, FILE* fpBin);
void funcionalidade4(FILE* fpBin);
void funcionalidade6(FILE* fpBin, char nomeDoCampo[17], char valor[100]);
int funcionalidade8(FILE* fpBin, int qtdRegistros);
int funcionalidade10(FILE* fpBin, FILE* fpIndex);
void funcionalidade12(FILE* fpBin, FILE* fpIndex, int valor);
int funcionalidade14(FILE* fpBin, FILE* fpIndex, int qtdRegistros);

#endif