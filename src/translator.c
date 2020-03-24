#include <stdio.h>
#include <stdlib.h>

#include "conversor.h"

// Funcao que insere a biblioteca adequada no header do arquivo
void wLibrary(FILE *arq){
	// Biblioteca default de C++
	fprintf(arq, "#include <iostream>\n\n");
}

// Funcao que insere o namespace adequado
void wNamespace(FILE *arq){
	// Namespace default de C++
	fprintf(arq, "using namespace std;\n\n");
}

// Funcao para ajustar a identacao
void ident(FILE *arq, int nivel){
	if(nivel <= 0){
		return;
	}
	for(int i = 0; i < nivel; i++){
		fprintf(arq, "	");
	}
}

void viewTree(tnode *no, int nivel){

	printf("%s, nivel %d\n", no->name, nivel);

	for(int i = 0; i < no->sIndex; i++){

		viewTree(no->slave[i], nivel + 1);
	}

	if(no->type != TINIT){
		no = no->master;
	}
}

void readTree(tnode *no, int nivel, FILE *arq){

	if(no->type == TOBJECT){
		ident(arq, nivel - 1);
		fprintf(arq, "class %s {\n", no->name);
	}

	for(int i = 0; i < no->sIndex; i++){

		readTree(no->slave[i], nivel + 1, arq);
	}

	if(no->type == TOBJECT){
		ident(arq, nivel - 1);
		fprintf(arq, "}\n");
	}

	if(no->type != TINIT){
		no = no->master;
	}
}

void buildProgram(tnode *inicio){
	// Abertura do arquivo de saida
	FILE *output = fopen("program.cpp","w");
	err_check(output);

	tnode* current = inicio;

	if(current->type == TINIT){
		wLibrary(output);
		wNamespace(output);
	}

	int level = 0; // Level eh um indicativo de quantos niveis ha na arvore
	viewTree(inicio, level);

	readTree(inicio, level, output);

	fclose(output);
}