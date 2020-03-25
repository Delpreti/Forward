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

void callFunction(char* nomeFunc){
	// Chama a funcao adequada
}

void buildProgram(tnode *inicio){

	// Partes da arvore Influenciam em partes distintas do codigo. Como resultado disso,
	// eh necessario escrever em diferentes posicoes do arquivo ao longo da arvore.
	// Para conseguir isto, abrimos 3 arquivos temporarios

	FILE *t_head = fopen("temphead.cpp","w+");
	err_check(t_head);

	FILE *t_class = fopen("tempclass.cpp","w+");
	err_check(t_class);

	FILE *t_main = fopen("tempmain.cpp","w+");
	err_check(t_main);

	tnode* current = inicio;

	if(current->type == TINIT){
		wLibrary(t_head);
		wNamespace(t_head);
	}

	// Essa funcao esta aqui dentro para nao ter que passar o nome do arquivo como parametro infinitas vezes
	void readTree(tnode *no, int nivel){

		if(no->type == TFUNCTION){
			callFunction(no->name);
		}

		if(no->type == TOBJECT){
			ident(t_class, nivel - 1);
			fprintf(t_class, "class %s {\n", no->name);
		}

		for(int i = 0; i < no->sIndex; i++){

			readTree(no->slave[i], nivel + 1);
		}

		if(no->type == TOBJECT){
			ident(t_class, nivel - 1);
			fprintf(t_class, "}\n\n");
		}

		if(no->type != TINIT){
			no = no->master;
		}
	}

	int level = 0; // Level eh um indicativo de quantos niveis ha na arvore

	viewTree(inicio, level); // Remover quando eu estiver confiante de que o programa funciona

	readTree(inicio, level);
	
	clearTree(inicio);

	// Abertura do arquivo de saida
	FILE *output = fopen("program.cpp","w");
	err_check(output);

	// Funcao que copia o conteudo dos arquivos temporarios para o arquivo de saida, e fecha o arquivo temporario
	void saveDismiss(FILE *arq){
		rewind(arq);
		char c;
		while ((c = fgetc(arq)) != EOF) {
	       fprintf(output, "%c", c);
	    }
	    fclose(arq);
	}

	saveDismiss(t_head);
	saveDismiss(t_class);
	saveDismiss(t_main);

	remove("temphead.cpp");
	remove("tempclass.cpp");
	remove("tempmain.cpp");

	fclose(output);
}