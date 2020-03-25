#include <stdio.h>
#include <stdlib.h>

#include "conversor.h"
#include "filemanager.h"

void inherit(tnode *no){
	// Este no recebe 2 objetos como parametros,
	// O primeiro objeto eh filho do segundo
	if(no->sIndex != 2){
		printf("Erro na passagem dos parametros da funcao /( {() is {() )\n");
		return;
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

void callFunction(tnode *no, int nivel){
	// Chama a funcao adequada, a ser implementado
}