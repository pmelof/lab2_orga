#include "cabecera3_orga.h"


/*VARIABLES GLOBALES*/
int LargoA = 0;
int* Arreglo;
int Desplazamiento=0;
int *sp;
Instruccion *ra;
Instruccion mCiclos[MAX_INSTRUCCIONES][5];	//matriz de ciclos.
Instruccion NOP;

/*
				IF ID EXE MEM WB
instrucciones
	...
*/


void iniciarMatrizCiclos()
{
	int i, j;
	for (i = 0; i < MAX_INSTRUCCIONES; i++)
	{
		for (j = 0; j < 5; j++)
		{
			mCiclos[i][j].index = NO_EXISTE;
			strcpy(mCiclos[i][j].nombre, "nada");
			strcpy(mCiclos[i][j].rd.nombre, REG_NULO);
			mCiclos[i][j].rd.valor = NO_EXISTE;
			strcpy(mCiclos[i][j].rs.nombre, REG_NULO);
			mCiclos[i][j].rs.valor = NO_EXISTE;
			strcpy(mCiclos[i][j].rt.nombre, REG_NULO);
			mCiclos[i][j].rt.valor = NO_EXISTE;
			mCiclos[i][j].imm = NO_EXISTE;
			strcpy(mCiclos[i][j].label, "");
			mCiclos[i][j].sgte = NULL;
		}
	}
}


void mostrarMatrizCiclos()
{
	int i, j;
	for (i = 0; i < MAX_INSTRUCCIONES; i++)
	{
		for (j = 0; j < 5; j++)
		{
		//	if (mCiclos[i][j].index != NO_EXISTE)
				printf("%d-%s\t", mCiclos[i][j].index, mCiclos[i][j].nombre);
		}
		//if (mCiclos[i][j].index != NO_EXISTE)
			printf("\n");
	}
}


void copiarInstruccion(int i, int j, Instruccion instruc)
{
	mCiclos[i][j].index = instruc.index;
	strcpy(mCiclos[i][j].nombre, instruc.nombre);
	strcpy(mCiclos[i][j].rd.nombre, instruc.rd.nombre);
	mCiclos[i][j].rd.valor = instruc.rd.valor;
	strcpy(mCiclos[i][j].rs.nombre, instruc.rs.nombre);
	mCiclos[i][j].rs.valor = instruc.rs.valor;
	strcpy(mCiclos[i][j].rt.nombre, instruc.rt.nombre);
	mCiclos[i][j].rt.valor = instruc.rt.valor;
	mCiclos[i][j].imm = instruc.imm;
	strcpy(mCiclos[i][j].label, instruc.label);
	mCiclos[i][j].sgte = instruc.sgte;
}


void agregarNOP(int i, int j)
{
	mCiclos[i][j].index = NO_EXISTE;
	strcpy(mCiclos[i][j].nombre, "NOP");
	strcpy(mCiclos[i][j].rd.nombre, REG_NULO);
	mCiclos[i][j].rd.valor = NO_EXISTE;
	strcpy(mCiclos[i][j].rs.nombre, REG_NULO);
	mCiclos[i][j].rs.valor = NO_EXISTE;
	strcpy(mCiclos[i][j].rt.nombre, REG_NULO);
	mCiclos[i][j].rt.valor = NO_EXISTE;
	mCiclos[i][j].imm = NO_EXISTE;
	strcpy(mCiclos[i][j].label, "");
	mCiclos[i][j].sgte = NULL;
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
		//fila = agregarInstruccion(*indice, lista, fila);
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
	//	fila++;
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
		i = indice->rs.valor;
		indice = buscarIndex(lista, i);		//salta a la dirección que contenga rs.
	}
	return indice;
}


Traza* generarTraza(Instruccion* lista, Registro* registros, Traza *t)
{
	int ciclos=0, indT=0;
	Instruccion* indice = lista;
	t = (Traza*)malloc(sizeof(Traza)*MAX_INSTRUCCIONES);

	while (indice != NULL)
	{
		/*
		hazard.
		*/
		t[indT].ciclo = ciclos;
		t[indT].instruccion = indice;
		if (indT >=2)	// puedo ejecutar.
		{
			ejecutarInstrucciones(indice, registros);
			indice = realizarSalto(indice, lista);
		}
		indice = indice->sgte;
		indT++;
		ciclos++;
	}
	return t;
}


void mostrarTraza(Traza* t)
{
	int i;
	for (i = 0; i < MAX_INSTRUCCIONES; i++)
	{
		printf("c:%d - %d - %s\n", t->ciclo, t->instruccion->index, t->instruccion->nombre);
	}
}

/*
int agregarInstruccion(Instruccion inst, Instruccion* lista, int fila)
{
	//VER CASO EN QUE SE SALTE A LA PRIMERA INSTRUCCIÓN.

	if (inst.index == 0)	
	{
		//primera instrucción, se guarda sin problemas.
		copiarInstruccion(0, 0, inst);	//IF
		copiarInstruccion(1, 1, inst);	//ID
		copiarInstruccion(2, 2, inst);	//EXE
		copiarInstruccion(3, 3, inst);	//MEM
		copiarInstruccion(4, 4, inst);	//WB
	}
	else
	{
		//buscar instrucción anterior.
		Instruccion* anterior = buscarIndex(lista, inst.index-1);
		
		//ver si hay forwarding.
		if (strcmp(anterior->rd.nombre, inst.rs.nombre)==0 || strcmp(anterior->rd.nombre, inst.rt.nombre)==0)
		{
			if (strcmp(anterior->nombre, "sw")==0 || strcmp(anterior->nombre, "lw")==0)
			{
				copiarInstruccion(fila, 0, inst);
				copiarInstruccion(fila+1, 1, inst);
				agregarNOP(fila+2, 2);
				agregarNOP(fila+3, 3);
				agregarNOP(fila+4, 4);

				copiarInstruccion(fila+1, 0, inst);
				copiarInstruccion(fila+2, 1, inst);
				copiarInstruccion(fila+3, 2, inst);
				copiarInstruccion(fila+4, 3, inst);
				copiarInstruccion(fila+5, 4, inst);
				fila++;
			}
			else
			{
				copiarInstruccion(fila, 0, inst);
				copiarInstruccion(fila+1, 1, inst);
				copiarInstruccion(fila+2, 2, inst);
				copiarInstruccion(fila+3, 3, inst);
				copiarInstruccion(fila+4, 4, inst);
			}
			//ver después el caso en que sea beq.
		}
		
	}
	return fila;
}
*/



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
	iniciarMatrizCiclos();
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
	t = generarTraza(lista, registros, t);
	mostrarTraza(t);
	printf("-----------------------\n");
	mostrarRegistros(registros);

	printf("\nFin del programa.\n");
	return 0;
}