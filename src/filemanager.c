#include <stdio.h>
#include <stdlib.h>

#include "conversor.h"
#include "funcoes.h"

// Partes da arvore Influenciam em partes distintas do codigo. Como resultado disso,
// eh necessario escrever em diferentes posicoes do arquivo ao longo da arvore.
// Para conseguir isto, abrimos 3 arquivos temporarios e definimos diversas funcoes
// para manusear estes arquivos da maneira mais desacoplada possivel.

// Funcao para visualizar a arvore
void viewTree(tnode *no, int nivel){

	printf("%s, nivel %d\n", no->name, nivel);

	for(int i = 0; i < no->sIndex; i++){

		viewTree(no->slave[i], nivel + 1);
	}

	if(no->type != TINIT){
		no = no->master;
	}
}

void createTempFiles(){
	FILE *t_head = fopen("temphead.cpp","w");
	err_check(t_head);

	fclose(t_head);

	FILE *t_class = fopen("tempclass.cpp","w");
	err_check(t_class);

	fclose(t_class);

	FILE *t_main = fopen("tempmain.cpp","w");
	err_check(t_main);

	fclose(t_main);
}

void writeHead(char* string){
	FILE *t_head = fopen("temphead.cpp","a");
	err_check(t_head);

	fprintf(t_head, "%s", string);

	fclose(t_head);
}

void identHead(int nivel){
	if(nivel <= 0){
		return;
	}
	for(int i = 0; i < nivel; i++){
		writeHead("	");
	}
}

void writeClass(char* string){
	FILE *t_class = fopen("tempclass.cpp","a");
	err_check(t_class);

	fprintf(t_class, "%s", string);

	fclose(t_class);
}

void identClass(int nivel){
	if(nivel <= 0){
		return;
	}
	for(int i = 0; i < nivel; i++){
		writeClass("	");
	}
}

void writeMain(char* string){
	FILE *t_main = fopen("tempmain.cpp","a");
	err_check(t_main);

	fprintf(t_main, "%s", string);

	fclose(t_main);
}

void identMain(int nivel){
	if(nivel <= 0){
		return;
	}
	for(int i = 0; i < nivel; i++){
		writeMain("	");
	}
}

void destroyTempFiles(){
	remove("temphead.cpp");
	remove("tempclass.cpp");
	remove("tempmain.cpp");
}

// Funcao que insere a biblioteca adequada no header do arquivo
void wLibrary(){
	// Biblioteca default de C++
	writeHead("#include <iostream>\n\n");
}

// Funcao que insere o namespace adequado
void wNamespace(){
	// Namespace default de C++
	writeHead("using namespace std;\n\n");
}

void wMainStart(){
	writeMain("int main(){\n");
}

void wMainEnd(){
	writeMain("}");
}

void readTree(tnode *no, int nivel){

	if(no->type == TINIT){ // This will execute once
		wLibrary();
		wMainStart();
	}

	if(no->type == TFUNCTION){
		callFunction(no, nivel);
	}

	if(no->type == TOBJECT){ //Badly written standard behaviour
		identClass(nivel - 1);
		writeClass("class ");
		writeClass(no->name);
		writeClass(" {\n");
	}

	if(no->type == TDISPLAY){ //Badly written standard behaviour
		identMain(nivel);
		writeMain("printf(\"");
		for(int i = 0; i < no->sIndex; i++){
			writeMain(no->slave[i]->name);
			writeMain(" ");
		}
		writeMain("\");\n");
	}

	for(int i = 0; i < no->sIndex; i++){ // The recursive loop for going deeper into the tree
		readTree(no->slave[i], nivel + 1);
	}

	if(no->type == TOBJECT){ //Badly written standard behaviour
		identClass(nivel - 1);
		writeClass("}\n\n");
	}

	if(no->type == TINIT){ // This will execute once
		wNamespace();
		wMainEnd();
	}

	if(no->type != TINIT){ // must remain last inside this function
		no = no->master;
	}
}

void buildProgram(tnode *inicio){

	createTempFiles();

	// Realiza a leitura da arvore, escreve nos arquivos temporarios e destroi a arvore.
	int level = 0; // Level eh um indicativo de quantos niveis ha na arvore
	viewTree(inicio, level); // Remover quando eu estiver confiante de que o programa funciona
	readTree(inicio, level);
	clearTree(inicio);

	// Abertura do arquivo de saida
	FILE *output = fopen("program.cpp","w");
	err_check(output);

	FILE *t_head = fopen("temphead.cpp","r");
	err_check(t_head);

	FILE *t_class = fopen("tempclass.cpp","r");
	err_check(t_class);

	FILE *t_main = fopen("tempmain.cpp","r");
	err_check(t_main);

	// Funcao que copia o conteudo do arquivo temporario para o arquivo de saida, e fecha o arquivo temporario.
	void saveDismiss(FILE *arq){
		char c;
		while ((c = fgetc(arq)) != EOF) {
	       fprintf(output, "%c", c);
	    }
	    fclose(arq);
	}

	saveDismiss(t_head);
	saveDismiss(t_class);
	saveDismiss(t_main);

	destroyTempFiles();

	fclose(output);
}