#ifndef TINYHMI_H
#define TINYHMI_H // comandos pro pré-processador não colar a biblioteca mais de uma vez no código

/*
 * 
 *  tinyhmi - Projeto Final de Programação Orientada a Objetos
 *
 *  nov. 2025
 *
 */

// definindo cores padrão do display caso as bibliotecas não tenham feito isso
#ifndef BLACK
	#define BLACK 0x0000
#endif

#ifndef WHITE
	#define WHITE 0xffff
#endif

// lembre-se: #include literalmente COLA seus arquivos no código-fonte
#include "widget.cpp"
#include "analog.cpp"
#include "digital.cpp"

#endif // TINYHMI_H
