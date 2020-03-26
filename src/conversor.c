#include <stdio.h>
#include <stdlib.h>

#include "filemanager.h"

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

// Funcao que traduz os simbolos dos brackets para os tipos de nodes da arvore
int defType(char t){
	switch(t){
		case '{' : return TOBJECT;
		case '/' : return TFUNCTION;
		case '#' : return TDISPLAY;
		case '"' : return TQUOTE;
		default : printf("erro de tipo nao existente\n"); return TERROR;
	}
}

int main() {

	// Abertura do arquivo de entrada (talvez deveria ser passado como parametro na main)
	FILE *input = fopen("texto.txt","r");
	err_check(input);

	// Cria o primeiro node
	tnode *raiz = new_tnode(TINIT, "root", NULL);
	tnode *atual = raiz;

	// Vai lendo e montando a arvore do programa
	int open = 0;
	int close = 0;
	int c; // nota: int para lidar com o EOF
    while ((c = fgetc(input)) != EOF) {

       	if(c == '{' || c == '/' || c == '#' || c == '"'){ // Provavelmente eu deveria comparar isso usando regex
			tnode *next = new_tnode(defType(c), "\0", atual);

			open++;
			c = fgetc(input);

			// Magia Negra
			if(atual->slave == NULL){
				atual->slave = malloc(sizeof(*(atual->slave)));
			}
			atual->slave[atual->sIndex] = next;

			atual->sIndex += 1;
			atual = next;
			continue;
		}

		if(c == ')'){
			close++;
			atual = atual->master;
			continue;
		}

		// Um espaco nao tem significado
		if(c == ' '){
			continue;
		}

		// Se nao eh um caracter de escopo, eh algum nome
		// A leitura desse nome eh feita concatenando a string
		char* palavra = malloc(20*sizeof(char));
		int i = 0;
		for(i = 0; (i < 20) && (atual->name[i] != '\0'); i++){
			palavra[i] = atual->name[i];
		}
		palavra[i] = c;
		while(i < 20){
			i++;

			c = fgetc(input);
			if(c == ' ' || c == ')'){
				palavra[i] = '\0';
				break;
			}
			palavra[i] = c;
		}
		atual->name = palavra;
		ungetc(c, input);



    }

    fclose(input);

    if(open != close){
    	printf("erro no arquivo de entrada. %d abertos e %d fechados\n", open, close);
    	clearTree(raiz);
    	return 1;
    }

	// Escrever arvore no arquivo de saida
	buildProgram(raiz);

	// Encerramento do programa
	return 0;
}