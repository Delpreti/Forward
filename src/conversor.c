#include <stdio.h>
#include <stdlib.h>

#include "translator.h"

// Convencao: variaveis em ingles, parametros de funcao em portugues

// Arvore do programa e enum definidos no arquivo .h

// Construtor de nodes
tnode *new_tnode(int tipo, char* nome, tnode *mestre){
	tnode *result = malloc(sizeof(tnode));
	result->type = tipo;
	result->name = nome;
	result->master = mestre;
	result->slave = NULL;
	result->sIndex = 0;

	return result;
}

// Funcao que verifica se houve erro na abertura do arquivo
void err_check(FILE *arq) {
	if (arq == NULL) {
       fprintf(stderr, "error opening file\n");
       exit(1);
	}
}

// limpa todos os slaves do node de maneira recursiva, e depois limpa o proprio node
void clearTree(tnode* no){
	free(no->name);
	for(int j = 0; j < no->sIndex; j++){
		clearTree(no->slave[j]);
	}
	free(no->slave);
	free(no);
}

int main() {

	// Abertura do arquivo de entrada (talvez deveria ser passado como parametro na main)
	FILE *input = fopen("texto.txt","r");
	err_check(input);

	// Cria o primeiro node
	tnode *raiz = new_tnode(TINIT, "root", NULL);
	tnode *atual = raiz;

	// Vai lendo e montando a arvore do programa
	int c; // nota: int para lidar com o EOF
    while ((c = fgetc(input)) != EOF) {

       	if(c == '{'){
			tnode *next = new_tnode(TOBJECT, NULL, atual);

			// Magia Negra, acho que nao esta funcionando direito isso nao
			if(atual->slave == NULL){
				atual->slave = malloc(sizeof(*(atual->slave)));
			}
			atual->slave[atual->sIndex] = next;

			atual->sIndex += 1;
			atual = next;
			continue;
		}

		if(atual->type == TOBJECT && c == '}'){
			atual = atual->master;
			continue;
		}

		// Um espaco nao tem significado
		if(c == ' '){
			continue;
		}

		// Se nao eh um caracter de escopo, eh algum nome
		char* palavra = malloc(20*sizeof(char));
		palavra[0] = c;
		for(int i = 1; i < 20; i++){
			
			c = fgetc(input);
			if(c == ' ' || c == '}'){
				palavra[i] = '\0';
				break;
			}
			palavra[i] = c;
		}
		atual->name = palavra;
		ungetc(c, input);
    }

    fclose(input);

	// Escrever arvore no arquivo de saida
	buildProgram(raiz);

	// Encerramento do programa
	clearTree(raiz);
	return 0;
}