#ifndef CONVERSOR_HEADER
#define CONVERSOR_HEADER

enum{TINIT, TOBJECT, TFUNCTION, TQUOTE, TINT, TDOUBLE, TERROR};

// Arvore do programa definida com nodes que guardam referencia entre si.
typedef struct tnode{
	// Essa estrutura define um node da arvore
	// Cada node possui um tipo (definir em um enum)
	int type;
	// um nome que geralmente vem do arquivo texto
	char* name;
	// Um ponteiro para os filhos (com contador) e um ponteiro para o pai
	struct tnode **slave;
	int sIndex;
	struct tnode *master;
}tnode;

void err_check(FILE *arq);

void clearTree(tnode* no);

#endif