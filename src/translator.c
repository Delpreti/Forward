#include <stdio.h>
#include <stdlib.h>

#include "conversor.h"

void buildProgram(tnode *inicio){
	// Abertura do arquivo de saida
	FILE *output = fopen("program.cpp","w");
	err_check(output);

	fclose(output);
}