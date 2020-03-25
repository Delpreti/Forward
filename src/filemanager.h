#ifndef FILEMANAGER_HEADER
#define FILEMANAGER_HEADER

#include "conversor.h"

// funcoes para escrever e identar nos arquivos temporarios respectivos
void writeHead(char* string);
void identHead(int nivel);

void writeClass(char* string);
void identClass(int nivel);

void writeMain(char* string);
void identMain(int nivel);

// Funcao que cria o programa a partir da arvore, separado da implementacao da arvore
void buildProgram(tnode *inicio);

#endif