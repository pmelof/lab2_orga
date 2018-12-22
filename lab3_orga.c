#include "cabecera3_orga.h"


/*VARIABLES GLOBALES*/
int LargoA = 0;
int* Arreglo;
int Desplazamiento=0;
int *sp;
Instruccion *ra;
Instruccion *NOP;


Instruccion* inicializarNOP()
{
	Instruccion* nuevo = (Instruccion*)malloc(sizeof(Instruccion));
	strcpy(nuevo->nombre, "NOP");
	//no es necesario inicializar los registros.
	nuevo->imm = NO_EXISTE;
	strcpy(nuevo->label, "");
	nuevo->sgte = NULL;
	return nuevo;
}

void quitarSaltoLinea(char *frase)
{
  char *ptr;
  ptr = strchr (frase, '\n');
  if (ptr)
    *ptr = '\0';
}


void iniciarRegistros(Registro* registros)
{
	int i=0;

	for (i = 0; i < TOTAL_REGISTRO; ++i)
	{
		registros[i].valor= 0;
	}

	strcpy(registros[0].nombre, "$zero");
	strcpy(registros[1].nombre, "$at");
	strcpy(registros[2].nombre, "$v0");
	strcpy(registros[3].nombre, "$v1");
	strcpy(registros[4].nombre, "$a0");
	strcpy(registros[5].nombre, "$a1");
	strcpy(registros[6].nombre, "$a2");
	strcpy(registros[7].nombre, "$a3");
	strcpy(registros[8].nombre, "$t0");
	strcpy(registros[9].nombre, "$t1");
	strcpy(registros[10].nombre, "$t2");
	strcpy(registros[11].nombre, "$t3");
	strcpy(registros[12].nombre, "$t4");
	strcpy(registros[13].nombre, "$t5");
	strcpy(registros[14].nombre, "$t6");
	strcpy(registros[15].nombre, "$t7");
	strcpy(registros[16].nombre, "$s0");
	strcpy(registros[17].nombre, "$s1");
	strcpy(registros[18].nombre, "$s2");
	strcpy(registros[19].nombre, "$s3");
	strcpy(registros[20].nombre, "$s4");
	strcpy(registros[21].nombre, "$s5");
	strcpy(registros[22].nombre, "$s6");
	strcpy(registros[23].nombre, "$s7");
	strcpy(registros[24].nombre, "$t8");
	strcpy(registros[25].nombre, "$t9");
	strcpy(registros[26].nombre, "$k0");
	strcpy(registros[27].nombre, "$k1");
	strcpy(registros[28].nombre, "$gp");
	strcpy(registros[29].nombre, "$sp");
	strcpy(registros[30].nombre, "$fp");
	strcpy(registros[31].nombre, "$ra");

	sp = Arreglo;
	printf("%ls\n", sp);
}



int buscarRegistro(char* reg, Registro* registros)
{
	int i=0;

	if (reg != NULL){
		for (i = 0; i < TOTAL_REGISTRO; ++i)
		{
			if(strcmp(reg, registros[i].nombre) == 0){
				return i;
			}
		}
		printf("Registro no encontrado.\n");
		return -1;
	}
	else{
		printf("Registro NULL.\n");
		return -1;
	}
}


void mostrarRegistros(Registro* registros){
	int i=0;
	printf("\n\nREGISTROS:\n");

	for (i=0; i<TOTAL_REGISTRO; i++)
	{
		printf("%s= %d\n", registros[i].nombre, registros[i].valor);
	}
	printf("FIN REGISTROS\n\n");
}


 /*TDA lista instrucciones*/
Instruccion* crearInstruccion(int index, char* nombre, Registro rd, Registro rs, Registro rt, int imm, char* label)
{
	Instruccion* nodito = (Instruccion*)malloc(sizeof(Instruccion));
	if (nodito != NULL)
	{
		nodito->index = index;
		strcpy(nodito->nombre, nombre);
		nodito->rd = rd;
		nodito->rs = rs;
		nodito->rt = rt;
		nodito->imm = imm;
		strcpy(nodito->label, label);
		nodito->sgte = NULL;
	}
	return nodito;
}


Instruccion* insertarInstruccion(Instruccion* lista, int index, char* nombre, Registro rd, Registro rs, Registro rt, int imm, char* label)
{
	Instruccion* aux;
	aux = lista;
	Instruccion* nueva = crearInstruccion(index, nombre, rd, rs, rt, imm, label);

	if (lista != NULL)
	{
		while (aux->sgte != NULL)
		{
			aux = aux->sgte;
		}
		aux->sgte = nueva;
	}
	else
	{
		return nueva;
	}
	return lista;
}


void mostrarLista(Instruccion* lista)
{
	Instruccion* indice = lista;
	printf("LISTA ENLAZADA:\n");
	while (indice != NULL)
	{
		printf("%d - %s %s %d, %s %d, %s %d, imm:%d, label:%s\n", indice->index, indice->nombre, 
			indice->rd.nombre, indice->rd.valor, indice->rs.nombre, indice->rs.valor, indice->rt.nombre, 
			indice->rt.valor, indice->imm, indice->label);
		indice = indice->sgte;
	}
	printf("\nFIN LISTA ENLAZADA.\n\n");
}


int buscarLabel(Instruccion* lista, char* etiqueta)
{
	Instruccion* indice = lista;
	while (indice != NULL)
	{
		if (strcmp(etiqueta, indice->label)==0)
			return indice->index;
		indice = indice->sgte;
	}
	return -1;
}


Instruccion* buscarIndex(Instruccion* lista, int i)
{
	Instruccion* indice = lista;
	while (indice != NULL)
	{
		if (i == indice->index)
			return indice;
		indice = indice->sgte;
	}
	return lista;
}

/*FIN TDA*/


void separarLwOSw(Registro* registros, Instruccion* lista, int index, char* nombre, Registro rd, char* rt)
{
	Registro RT, RS;
	int caracteres=0, i=0, pos;
	char num[TAM_PALABRA];
	char regReal[TAM_PALABRA];
	char letra[TAM_PALABRA];
	strcpy(num, "");
	strcpy(regReal, "");
	strcpy(letra, "");

	while (rt[i] != '(')
	{
		sprintf(letra,  "%c", rt[i]);
		strcat(num, letra);
		caracteres++;
		i++;
	}

	caracteres++;
	while (rt[caracteres] != ')')
	{
		sprintf(letra,  "%c", rt[caracteres]);
		strcat(regReal, letra);
		caracteres++;
	}

	pos = buscarRegistro(regReal, registros);
	if (pos != -1)
		RT = registros[pos];

	strcpy(RS.nombre, REG_NULO); 

	lista = insertarInstruccion(lista, index, nombre, rd, RS, RT, atoi(num), "");
}


Instruccion* leerEntrada(char* nombre, Registro* registros, Instruccion* lista)
{
	FILE* archivo = NULL;
	archivo = fopen(nombre, "rb");
	if(archivo == NULL){
		printf("\n**(No se encontro el archivo %s)**\n\n", nombre);
		exit(1);
	}

	//asumiendo instruc rd rs rt
	char instruc[TAM_PALABRA];
	char rd[TAM_PALABRA];
	char rs[TAM_PALABRA];
    char rt[TAM_PALABRA];
    int posRD =-1, posRS=-1, posRT=-1, index=0;
    Registro RD, RS, RT;

    if (NULL!= archivo)
    {
    	while (feof(archivo)!=1)
    	{
    	//	strcpy(instruc, "");
			fscanf(archivo, "%s", instruc);//obtengo una instruccion.

			if (strcmp(instruc, "add") == 0 || strcmp(instruc, "sub") == 0
				|| strcmp(instruc, "mul") == 0 || strcmp(instruc, "div") == 0)
			{
				fscanf(archivo, "%s", rd);	
		    	rd[strlen(rd)-1]= '\0';	//quitar coma.
		    	fscanf(archivo, "%s", rs);
		    	rs[strlen(rs)-1]= '\0';	//quitar coma.
		    	fscanf(archivo, "%s", rt);

		    	posRD = buscarRegistro(rd, registros);
		    	posRS = buscarRegistro(rs, registros);
		    	posRT = buscarRegistro(rt, registros);

		    	RD = registros[posRD];
		    	RS = registros[posRS];
		    	RT = registros[posRT];

		    	lista = insertarInstruccion(lista, index, instruc, RD, RS, RT, NO_EXISTE, "");
			}
			else if (strcmp(instruc, "addi") == 0 || strcmp(instruc, "subi") == 0
				|| strcmp(instruc, "addiu") == 0)
			{
				fscanf(archivo, "%s", rd);	
		    	rd[strlen(rd)-1]= '\0';	//quitar coma.
		    	fscanf(archivo, "%s", rs);
		    	rs[strlen(rs)-1]= '\0';	//quitar coma.
		    	fscanf(archivo, "%s", rt); // imm.

		    	posRD = buscarRegistro(rd, registros);
		    	posRS = buscarRegistro(rs, registros);

		    	RD = registros[posRD];
		    	RS = registros[posRS];
		    	strcpy(RT.nombre, REG_NULO); 

		    	lista = insertarInstruccion(lista, index, instruc, RD, RS, RT, atoi(rt), "");
			}
			else if (strcmp(instruc, "beq") == 0 || strcmp(instruc, "bne") == 0
				|| strcmp(instruc, "blt") == 0 || strcmp(instruc, "bgt") == 0)
			{
				fscanf(archivo, "%s", rd);	
		    	rd[strlen(rd)-1]= '\0';	//quitar coma.
		    	fscanf(archivo, "%s", rs);
		    	rs[strlen(rs)-1]= '\0';	//quitar coma.
		    	fscanf(archivo, "%s", rt); // label.

		    	posRD = buscarRegistro(rd, registros);
		    	posRS = buscarRegistro(rs, registros);

		    	RD = registros[posRD];
		    	RS = registros[posRS];
		    	strcpy(RT.nombre, REG_NULO); 

		    	lista = insertarInstruccion(lista, index, instruc, RD, RS, RT, NO_EXISTE, rt);
			}
			else if (strcmp(instruc, "j") == 0 || strcmp(instruc, "jal") == 0)
			{
				fscanf(archivo, "%s", rt); // label.
				strcpy(RD.nombre, REG_NULO);
				strcpy(RS.nombre, REG_NULO);
				strcpy(RT.nombre, REG_NULO);

				lista = insertarInstruccion(lista, index, instruc, RD, RS, RT, NO_EXISTE, rt);
			}
			else if (strcmp(instruc, "jr") == 0)
			{
				fscanf(archivo, "%s", rs);
				strcpy(RD.nombre, REG_NULO);
				strcpy(RT.nombre, REG_NULO);
				posRS = buscarRegistro(rs, registros);
				RS = registros[posRS];

				lista = insertarInstruccion(lista, index, instruc, RD, RS, RT, NO_EXISTE, "");
			}
			else if (strcmp(instruc, "lw") == 0 || strcmp(instruc, "sw") == 0)
			{
				fscanf(archivo, "%s", rd);	
		    	rd[strlen(rd)-1]= '\0';	//quitar coma.
				fscanf(archivo, "%s", rt); // dir.

				posRD = buscarRegistro(rd, registros);
				RD = registros[posRD];

				separarLwOSw(registros, lista, index, instruc, RD, rt);
			}
			else
			{
				char* etiqueta;
				etiqueta = strtok(instruc, ":");
				strcpy(RD.nombre, REG_NULO);
				strcpy(RS.nombre, REG_NULO);
				strcpy(RT.nombre, REG_NULO);
		    //	instruc[strlen(rd)-1]= '\0';	//quitar coma.
		    	//ver como elimino eso.. lo hace mal.
				lista = insertarInstruccion(lista, index, "", RD, RS, RT, NO_EXISTE, etiqueta); //es label.
			}
			index++;
		}
    }
	else
		printf("No se encontro archivo.\n");
	fclose(archivo);
	return lista;
}


void ejecutarInstrucciones(Instruccion *lista, Registro* registros)
{
	int RD, RS, RT;
	if (strcmp(lista->nombre, "add") == 0)
	{
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);
		RT = buscarRegistro(lista->rt.nombre, registros);
		registros[RD].valor = registros[RS].valor + registros[RT].valor;
	}

	else if (strcmp(lista->nombre, "sub") == 0)
	{
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);
		RT = buscarRegistro(lista->rt.nombre, registros);
		registros[RD].valor = registros[RS].valor - registros[RT].valor;
	}

	else if (strcmp(lista->nombre, "mul") == 0)
	{
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);
		RT = buscarRegistro(lista->rt.nombre, registros);
		registros[RD].valor = registros[RS].valor * registros[RT].valor;
	}

	else if (strcmp(lista->nombre, "div") == 0)
	{
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);
		RT = buscarRegistro(lista->rt.nombre, registros);

		registros[RD].valor = registros[RS].valor / registros[RT].valor;
    }
    else if (strcmp(lista->nombre, "addi") == 0)
	{
		if (strcmp(lista->rd.nombre, "$sp")==0 && strcmp(lista->rs.nombre, "$sp")==0)
		{
			if (lista->imm < 0)
			{
				//asigno memoria.
				lista->imm = lista->imm*-1;
				Arreglo = (int*)malloc(sizeof(int)*(lista->imm/4));
				LargoA = LargoA + lista->imm/4;
			}
			else
			{
				Desplazamiento = Desplazamiento + lista->imm/4;
			}
		}
		else
		{
			RD = buscarRegistro(lista->rd.nombre, registros);
			RS = buscarRegistro(lista->rs.nombre, registros);
			registros[RD].valor = registros[RS].valor + lista->imm;
		}
	}

	else if (strcmp(lista->nombre, "subi") == 0)
	{
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);
		registros[RD].valor = registros[RS].valor - lista->imm;
	}

	else if (strcmp(lista->nombre, "addiu") == 0)
	{
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);
		registros[RD].valor = registros[RS].valor + lista->imm;
	}

	else if (strcmp(lista->nombre, "beq") == 0)
	{
		//beq no usa rt, ahí pondré el valor de saltar o no.
		//saltar = 1, no saltar = 0.
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);

		if (registros[RD].valor == registros[RS].valor)
			lista->rt.valor = 1;
		else
			lista->rt.valor = 0;
	}
	
	else if (strcmp(lista->nombre, "bne") == 0)
	{
		//bne no usa rt, ahí pondré el valor de saltar o no.
		//saltar = 1, no saltar = 0.
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);

		if (registros[RD].valor != registros[RS].valor)
			lista->rt.valor = 1;
		else
			lista->rt.valor = 0;
	}

	else if (strcmp(lista->nombre, "blt") == 0)
	{
		//blt no usa rt, ahí pondré el valor de saltar o no.
		//saltar = 1, no saltar = 0.
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);

		if (registros[RD].valor < registros[RS].valor)
			lista->rt.valor = 1;
		else
			lista->rt.valor = 0;
	}

	else if (strcmp(lista->nombre, "bgt") == 0)
	{
		//bgt no usa rt, ahí pondré el valor de saltar o no.
		//saltar = 1, no saltar = 0.
		RD = buscarRegistro(lista->rd.nombre, registros);
		RS = buscarRegistro(lista->rs.nombre, registros);

		if (registros[RD].valor > registros[RS].valor)
			lista->rt.valor = 1;
		else
			lista->rt.valor = 0;
	}

	else if (strcmp(lista->nombre, "lw") == 0)
	{
		if (strcmp(lista->rt.nombre, "$sp")==0)
		{
			RD = buscarRegistro(lista->rd.nombre, registros);
			registros[RD].valor = Arreglo[lista->imm/4 + Desplazamiento];
		}
	}

	else if (strcmp(lista->nombre, "sw") == 0)
	{
		if (strcmp(lista->rt.nombre, "$sp")==0)
		{
			RD = buscarRegistro(lista->rd.nombre, registros);
			Arreglo[lista->imm/4 + Desplazamiento] = registros[RD].valor;
		}		
	}

	else if (strcmp(lista->nombre, "jal") == 0)
	{
		ra = lista->sgte;
	}
	//si es jr o j, no se hace nada.
}


//monociclo.
void ejecutarPrograma(Instruccion* lista, Registro* registros)
{
	int i=0;
	Instruccion* indice = lista;
	printf("PROGRAMA:\n");
	while (indice != NULL)
	{
		ejecutarInstrucciones(indice, registros);
		printf("%s\n", indice->nombre);

		if (strcmp(indice->nombre, "beq")==0 || strcmp(indice->nombre, "bne")==0 || 
			strcmp(indice->nombre, "blt")==0 || strcmp(indice->nombre, "bgt")==0)
		{
			if (indice->rt.valor == 1)		// se salta.
			{
				i = buscarLabel(lista, indice->label);
				indice = buscarIndex(lista, i + 1);		//salta a la instrucción después de la etiqueta.
			}		
		}
		else if (strcmp(indice->nombre, "j")==0 || strcmp(indice->nombre, "jal")==0)
		{
			i = buscarLabel(lista, indice->label);
			indice = buscarIndex(lista, i);		//salta a la instrucción después de la etiqueta.
		}
		else if (strcmp(indice->nombre, "jr")==0)
		{
			int i;
			if (strcmp(indice->rs.nombre, "$ra")==0)
			{
				indice = ra;
			}
			i = indice->rs.valor;
			indice = buscarIndex(lista, i);		//salta a la dirección que contenga rs.
		}
		
	/*	printf("%d - %s %s %d, %s %d, %s %d, imm:%d, label:%s\n", indice->index, indice->nombre,  
			indice->rd.nombre, indice->rd.valor, indice->rs.nombre, indice->rs.valor, indice->rt.nombre, 
			indice->rt.valor, indice->imm, indice->label);*/
		indice = indice->sgte;
	}
	printf("\nFIN PROGRAMA.\n\n");
}


Instruccion* realizarSalto(Instruccion* indice, Instruccion* lista)
{
	int i;
	if (strcmp(indice->nombre, "beq")==0 || strcmp(indice->nombre, "bne")==0 || 
		strcmp(indice->nombre, "blt")==0 || strcmp(indice->nombre, "bgt")==0)
	{
		if (indice->rt.valor == 1)		// se salta.
		{
			i = buscarLabel(lista, indice->label);
			indice = buscarIndex(lista, i + 1);		//salta a la instrucción después de la etiqueta.
		}		
	}
	else if (strcmp(indice->nombre, "j")==0 || strcmp(indice->nombre, "jal")==0)
	{
		i = buscarLabel(lista, indice->label);
		indice = buscarIndex(lista, i);		//salta a la instrucción después de la etiqueta.
	}
	else if (strcmp(indice->nombre, "jr")==0)
	{
		int i;
		if (strcmp(indice->rs.nombre, "$ra")==0)
		{
			indice = ra;
		}
		else
		{
			i = indice->rs.valor;
			indice = buscarIndex(lista, i);		//salta a la dirección que contenga rs.
		}
	}
	return indice;
}


Traza* generarTraza(Instruccion* lista, Registro* registros)
{
	int ciclos=0, indT=0;
	Instruccion* indice = lista;
	Traza *t = (Traza*)malloc(sizeof(Traza)*MAX_INSTRUCCIONES);

	while (indice != NULL && indT < MAX_INSTRUCCIONES)
	{
		if (strcmp(indice->nombre, "")==0) {
			indice = indice->sgte;
			continue;
		}
		/*
		hazard.
		*/
		t[indT].ciclo = ciclos;
		t[indT].instruccion = indice;
		t[indT].valido = 1;
		if (indT >=2)	// puedo ejecutar.
		{
			Instruccion* p;
			Instruccion* p2;

			p = t[indT-2].instruccion;
			ejecutarInstrucciones(p, registros);
			p2 = realizarSalto(p, lista);
			if (p != p2) 
			{
				indice = p2;
				if (strcmp(p->nombre, "beq")==0 || strcmp(p->nombre, "bne")==0 || 
				strcmp(p->nombre, "blt")==0 || strcmp(p->nombre, "bgt")==0)
				{
					t[indT-1].instruccion = NOP;
					t[indT].instruccion = NOP;
				}
				else if (strcmp(p->nombre, "j")==0 || strcmp(p->nombre, "jal")==0)
				{
					t[indT-1].instruccion = NOP;
					t[indT].instruccion = NOP;
					ciclos--;
					t[indT].valido = 0;
				}
				else if (strcmp(p->nombre, "jr")==0)
				{
					t[indT-1].instruccion = NOP;
				}
				else
					printf("ERROR, NO DEBERÍA ENTRAR AQUI.");
			}
		}
		indice = indice->sgte;
		indT++;
		ciclos++;
	}
	t[indT].instruccion = NULL;
	return t;
}


void mostrarTraza(Traza* t)
{
	int i=0;
	while (t[i].instruccion != NULL)
	{
		if (t[i].valido == 1)
		{
			printf("c:%d - %d - %s\n", t[i].ciclo, t[i].instruccion->index, t[i].instruccion->nombre);
		}
		i++;
	}
}


int main(int argc, char const *argv[])
{
	printf("\n.......................................\n");
    printf(".                                     .\n");
    printf(".            Laboratorio 3            .\n");
    printf(".              Pipeline               .\n");
    printf(".......................................\n");
    printf(".     Organización de Computadores    .\n");
    printf(".                                     .\n");
    printf(".       Patricia Melo Fuenzalida      .\n");
    printf(".......................................\n");

	Registro registros[TOTAL_REGISTRO];
	iniciarRegistros(registros);
	Traza *t;
	NOP = inicializarNOP();
	Instruccion* lista = NULL;
	char entrada[TAM_PALABRA];
//	char salida[TAM_PALABRA];
//	char salida2[TAM_PALABRA];
	//FILE* archivo = NULL;

	printf("Ingrese nombre del archivo de entrada (ejemplo: Entrada.txt):\n");
	fgets(entrada, TAM_PALABRA, stdin);
	quitarSaltoLinea(entrada);
	/*printf("Ingrese nombre del archivo de salida para guardar el resultado:\n");
	fgets(salida, TAM_PALABRA, stdin);
	quitarSaltoLinea(salida);
	printf("Ingrese nombre del archivo de salida para guardar las etapas del pipeline:\n");
	fgets(salida2, TAM_PALABRA, stdin);
	quitarSaltoLinea(salida2);*/

	lista = leerEntrada(entrada, registros, lista);
	//mostrarLista(lista);

	//archivo = fopen(salida2, "w");
	//mostrarEtapas(archivo);
	//fclose(archivo);

	ejecutarPrograma(lista, registros);
	printf("-----------------------\n");
	t = generarTraza(lista, registros);
	mostrarTraza(t);
	printf("-----------------------\n");
	mostrarRegistros(registros);

	printf("\nFin del programa.\n");
	return 0;
}