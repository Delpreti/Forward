#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conversor.h"
#include "filemanager.h"

void display(tnode *no){
	// Este no exibe como string tudo o que for slave (quantidade variavel).
	// O comportamento padrao eh concatenar todos os slave->name (recursivamente)
	// E depois jogar tudo num printf().
	// Se houver um valor numerico [(), sera exibido o valor/resultado ao inves do name.
	if(no->sIndex == 0){
		printf("Warning: nada a ser exibido por #()\n");
		return;
	}
}

void inherit(tnode *no){
	// Este no recebe 2 objetos como parametros,
	// O primeiro objeto eh filho do segundo


}

void is_true(tnode *no){
	// Este no recebe 2 objetos como parametros
	// O segundo eh o nome de uma variavel private do primeiro(objeto), que sera setada para true.
}

void is_false(tnode *no){
	// Este no recebe 2 objetos como parametros
	// O segundo eh o nome de uma variavel private do primeiro(objeto), que sera setada para false.
}

void is(tnode *no){
	// Este no recebe 2 objetos como parametros
	if(no->sIndex != 2){
		printf("Erro na passagem dos parametros da funcao /( () is () )\n");
		return;
	}
	// Disambiguation
	if(no->slave[0]->type == TOBJECT && no->slave[1]->type == TOBJECT){
		inherit(no);
	}

	/*
	if(no->slave[0]->type == TQUOTE && no->slave[1]->type == TINT){
		assign(no);
	}*/

	if(no->slave[0]->type == TOBJECT && no->slave[1]->type == TQUOTE){
		is_true(no);
	}
}

void each(tnode *no){
	// Este no recebe 1 quote e 1 array como parametros,
	// A quote eh o nome que os elementos do array irao receber no programa
	// A tipagem dos elementos eh definida na tipagem do no do array
	if(no->sIndex != 2){
		printf("Erro na passagem dos parametros da funcao /( each \"() in *() )\n");
		return;
	}

}

void set(tnode *no){
	// Este no recebe 1 quote e 1 valor como parametros,
	// A quote eh o nome da variavel que vai receber o valor
	if(no->sIndex != 2){
		printf("Erro na passagem dos parametros da funcao /( set \"() to [() )\n");
		return;
	}
}

void and(tnode *no){
	// Recebe uma quantidade variavel de parametros
	// Funcao depende do contexto.
}

void multiply(tnode *no);

void sum(tnode *no);

void subtract(tnode *no);

void divide(tnode *no);


// As funções definidas acima precisam ser registradas para uso, alterando as 3 proximas linhas
char* funcoes[] = {"is", "isnot", "eachin", "setto", "and", "+", "-", "*", "/"};
void (*chamador[]) (tnode *no) = {&is, &is_false, &each, &set, &and, &sum, &subtract, &multiply, &divide}
const int quant = 9

// Chamador generico
void callFunction(tnode *no, int nivel){
	for(int i = 0; i < quant; i++){
		if(strcmp(no->name, funcoes[i]) == 0){
			(*chamador[i]) (no);
		}
	}
}
