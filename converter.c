#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// O objetivo deste programa eh pegar um arquivo de texto escrito em linguagem natural e converter para C++
// Atualmente, o texto precisa ser pre-formatado segundo alguns criterios. Esses criterios são:
// {objetos}, [valores ou expressoes numericas], /funcoes/, (instancias ou apelidos), "Quotes".
// O prefixo # indica algo que deve ser exibido (chamada de printf no programa.cpp de saida, nao aqui)
// Por enquanto sao essas regras, se for necessario criar mais marcacoes como essas elas serao criadas.

/*	O resultado das expressoes pode ter multiplos tipos.
	Como o tipo a ser retornado não pode ser previsto antes da leitura (funções retornam void),
	o valor de retorno eh lancado nas variaveis globais abaixo.
	A variavel sugestivamente chamada "delegate" eh 0(zero) se não houver valor a ser retornado.
	Se houver, delegate precisa assumir o valor adequado,
	conforme orientado nos comentarios ao lado das variaveis globais.  */
int resInt; 		// 1
double resDouble;	// 2
char* resString;	// 3
//bool resBool;		// 4 // So quando eu for mexer com C++

int delegate = 0;

// Funcao que verifica se houve erro na abertura do arquivo
void err_check(FILE *fp) {
	if (fp == NULL) {
       fprintf(stderr, "error opening file\n");
       exit(1);
	}
}

// Prototipo da funcao que envelopa e distribui os elementos da arvore sintatica
// Precisa ser definido aqui pois eh chamado multiplas vezes pelas outras funcoes (uma coisa dentro da outra)
void wWrap(FILE *in, FILE *out, char* word, int lvl, int *cc);

// Funcao que insere as bibliotecas adequadas no arquivo
void wLibrary(FILE *s){
	fprintf(s, "#include <iostream>\n\n");
}

// Funcao que insere o namespace adequado
void wNamespace(FILE *s){
	fprintf(s, "using namespace std;\n\n");
}

// Funcao que faz o fechamento dos brackets do programa em c++
void wClose(FILE *s){
	fprintf(s, "}\n\n");
}

// Funcao que abre uma classe para criar o objeto
void wClass(FILE *in, FILE *out, char* name, int lvl, int *cc){
	fprintf(out, "class ");

	int open = 0;

	*cc += 1;
	int start = *cc;

	for(; name[*cc] != '}'; *cc = *cc + 1){
		if(*cc >= 19){
			// Erro de escrita
			printf("Ocorreu um erro no fechamento de classe\n");
			return;
		}
		if(name[*cc] == '\0'){ // Trocar por strlen() ?
			// A string terminou antes do bracket de fechamento, ou seja, o usuario inseriu coisas dentro da classe
			// Devemos entao abrir a classe
			fprintf(out, " {\n\n");
			open = 1;
			// E depois descer um nivel na hierarquia. Ainda nao implementado corretamente
			fscanf(in, "%s", name);
			wWrap(in, out, name, 0, cc);
			break;
		}
		if(name[*cc] == ' '){
			// A leitura de strings deveria desconsiderar os espacos em branco
			printf("Ocorreu um erro bizarro\n");
			return;
		}
		fprintf(out, "%c", name[*cc]);
	}

	if(open == 0){
		fprintf(out, " {\n\n");
		open = 1;
	}

	// Cria um construtor vazio
	fprintf(out, "	public:\n		void ");
	for(int i = start; i < *cc; i++){
		fprintf(out, "%c", name[i]);
	}
	fprintf(out, "(){}\n");

	if(name[*cc] == '}'){
		wClose(out);
		*cc = *cc + 1;
	}
}

void wExec(char* fName){
	// Na teoria faz varias coisas
}

// Funcao que cria uma funcao (isso vai ser doido de fazer em bicho), por enquanto reconhece mas nao faz nada ainda
void wFunction(FILE *in, FILE *out, char* name, int lvl, int *cc){

	*cc += 1;
	// verifica se o item deve ser considerado um parametro da funcao ou nao
	int test = -1;
	while(test != *cc){
		test = *cc;
		wWrap(in, out, name, lvl, cc);
		if(test != *cc){
			fscanf(in, "%s", name);
			*cc = 0;
		}
	}

	// Guardar o nome da funcao
	char* funcName = malloc(20*sizeof(char));
	strcpy(funcName, name);

	// verifica se o item deve ser considerado um parametro da funcao ou nao
	test = -1;
	while(test != *cc){
		test = *cc;
		wWrap(in, out, name, lvl, cc);
		if(test != *cc){
			fscanf(in, "%s", name);
			*cc = 0;
		}
	}

	// Chama a funcao adequada
	wExec(funcName);
	free(funcName);

}

// Funcao que gera o codigo referente ao valor a ser exibido
void wValue(FILE *in, FILE *out, char* name, int lvl){
	switch(delegate){
		case 1 : fprintf(out, "printf(\"%d\\n\");", resInt);
		case 2 : fprintf(out, "printf(\"%.2lf\\n\");", resDouble);
		case 3 : fprintf(out, "printf(\"%s\\n\");", resString);
	}

}

void wMainStart(FILE *s){
	fprintf(s, "int main(){\n\n");
}

void wMainEnd(FILE *s){
	fprintf(s, "	return 0;\n}");
}

void wDisplay(FILE *in, FILE *out, char* name, int lvl){

}

// Funcao que poe uma coisa dentro da outra e cria a arvore de execucao do programa
// Recebe como parametros:
// 1- arquivos de entrada e saida,
// 2- uma string lida do arquivo texto, e
// 3- o nivel na arvore de chamadas do programa
void wWrap(FILE *in, FILE *out, char* word, int lvl, int *cc){
	
    // Os objetos sao colocados entre chaves {objeto}
	if(word[lvl] == '{'){
		wClass(in, out, word, lvl + 1, cc);
	}

	// As funcoes sao colocadas entre barras /funcao/ (obs.: nao vai gerar exatamente uma funcao no codigo cpp, meio dificil de explicar)
	if(word[lvl] == '/'){
		wFunction(in, out, word, lvl + 1, cc);
	}

	// Aquilo que o usuario deseja que seja exibido na tela recebe o prefixo #
	if(word[lvl] == '#'){
		wDisplay(in, out, word, lvl + 1);
	}

	// Valores numericos seguem a notacao de array [numero]
	if(word[lvl] == '['){
		wValue(in, out, word, lvl + 1);
	}

	/*else {
		fprintf(output, "%s\n", palavra);
	}*/
}


int main() {

	FILE *f = fopen("texto.txt","r");
	err_check(f);

	FILE *output = fopen("programa.cpp","w");
	err_check(output);

	FILE *temp = fopen("temp.cpp","w+");
	err_check(temp);

	/*
	int read;

	// apenas escreve o valor lido na Shell
	while ((read = fgetc(f)) != EOF) { // standard C I/O file reading loop
    	putchar(read);
    }
    */

	wLibrary(output);
	wNamespace(output);
	wMainStart(temp);

	// Aloca espaco para leitura e declara uma variavel para guardar o indice do caracter que esta sendo lido no momento
	char* palavra = malloc(20*sizeof(char));
	int cc = 0; // cc stands for Current Character

	while(!feof(f)){
		fscanf(f, "%s", palavra);
		wWrap(f, output, palavra, 0, &cc);
	}

	char c;
	rewind(temp);
	while ((c = fgetc(temp)) != EOF) { // standard C I/O file reading loop
       fprintf(output, "%c", c);
    }

	wMainEnd(output);

	free(palavra);

	fclose(temp);
    fclose(f);
    fclose(output);

	return remove("temp.cpp");
}