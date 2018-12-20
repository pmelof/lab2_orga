/*
Laboratorio 3 de Organizacipon de Computadores.
Patricia Melo Fuenzalida.
*/


/*Librerías a ocupar*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*DEFINICIONES*/
#define TAM_PALABRA 32
#define TAM_REGISTRO 32
#define TOTAL_REGISTRO 32
#define NO_EXISTE -99
#define REG_NULO ""


/*ESTRUCTURAS*/

typedef struct Registro
{
	char nombre[TAM_REGISTRO];
	int valor;
}Registro;


typedef struct Instruccion
{
	int  index;
	char nombre[TAM_PALABRA];
	Registro rd;
	Registro rs;
	Registro rt;
	int imm;
	char label[TAM_PALABRA];
	struct Instruccion* sgte; 
}Instruccion;


/*FUNCIONES*/

void quitarSaltoLinea(char *frase);
void iniciarRegistros(Registro* registros);
int buscarRegistro(char* reg, Registro* registros);
Instruccion* crearInstruccion(int index, char* nombre, Registro rd, Registro rs, Registro rt, int imm, char* label);
Instruccion* insertarInstruccion(Instruccion* lista, int index, char* nombre, Registro rd, Registro rs, Registro rt, int imm, char* label);
void mostrarLista(Instruccion* lista);
int buscarLabel(Instruccion* lista, char* etiqueta);
Instruccion* buscarIndex(Instruccion* lista, int i);
void ejecutarPrograma(Instruccion* lista, Registro* registros);
void separarLwOSw(Registro* registros, Instruccion* lista, int index, char* nombre, Registro rd, char* rt);
Instruccion* leerEntrada(char* nombre, Registro* registros, Instruccion* lista);
void ejecutarInstrucciones(Instruccion *lista, Registro* registros);

