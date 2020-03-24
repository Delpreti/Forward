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

void viewTree(tnode *no, int nivel){

	printf("%s, nivel %d\n", no->name, nivel);

	for(int i = 0; i < no->sIndex; i++){

		viewTree(no->slave[i], nivel + 1);
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



	fclose(output);
}