#include "cabecera3_orga.h"


/*VARIABLES GLOBALES*/
int LargoA = 0;
int* Arreglo;
int Desplazamiento=0;
int *sp;
Instruccion *ra;
Instruccion *NOP;
Hazard hazards[MAX_INSTRUCCIONES];


Instruccion* inicializarNOP()
{
	Instruccion* nuevo = (Instruccion*)malloc(sizeof(Instruccion));
	strcpy(nuevo->nombre, "NOP");
	nuevo->index = NO_EXISTE;
	nuevo->nroInstruccion = NO_EXISTE;
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
		printf("Registro no encontrado. %s\n", reg);
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
Instruccion* crearInstruccion(int index, int nroInstruccion, char* nombre, Registro rd, Registro rs, Registro rt, int imm, char* label)
{
	Instruccion* nodito = (Instruccion*)malloc(sizeof(Instruccion));
	if (nodito != NULL)
	{
		nodito->index = index;
		nodito->nroInstruccion = nroInstruccion;
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


Instruccion* insertarInstruccion(Instruccion* lista, int index, int nroInstruccion, char* nombre, Registro rd, Registro rs, Registro rt, int imm, char* label)
{
	Instruccion* aux;
	aux = lista;
	Instruccion* nueva = crearInstruccion(index, nroInstruccion, nombre, rd, rs, rt, imm, label);

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
		printf("%d %d- %s %s %d, %s %d, %s %d, imm:%d, label:%s\n", indice->index, indice->nroInstruccion, indice->nombre, 
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
		if (strcmp(etiqueta, indice->label)==0 && strcmp(indice->nombre, "")==0)
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


void separarLwOSw(Registro* registros, Instruccion* lista, int index, int nroInstruccion, char* nombre, Registro rd, char* rt)
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

	strcpy(RS.nombre, nombre); 

	lista = insertarInstruccion(lista, index, nroInstruccion, nombre, rd, RS, RT, atoi(num), "");
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
    int posRD =-1, posRS=-1, posRT=-1, index=0, nroInstruccion=0;
    Registro RD, RS, RT;

    if (NULL!= archivo)
    {
		fscanf(archivo, "%s", instruc);//obtengo una instruccion.
    	while (!feof(archivo))
    	{
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

		    	lista = insertarInstruccion(lista, index, nroInstruccion, instruc, RD, RS, RT, NO_EXISTE, "");
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
		    	strcpy(RT.nombre, instruc); 

		    	lista = insertarInstruccion(lista, index, nroInstruccion, instruc, RD, RS, RT, atoi(rt), "");
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
		    	strcpy(RT.nombre, instruc); 

		    	lista = insertarInstruccion(lista, index, nroInstruccion, instruc, RD, RS, RT, NO_EXISTE, rt);
			}
			else if (strcmp(instruc, "j") == 0 || strcmp(instruc, "jal") == 0)
			{
				fscanf(archivo, "%s", rt); // label.
				strcpy(RD.nombre, instruc);
				strcpy(RS.nombre, instruc);
				strcpy(RT.nombre, instruc);

				lista = insertarInstruccion(lista, index, nroInstruccion, instruc, RD, RS, RT, NO_EXISTE, rt);
			}
			else if (strcmp(instruc, "jr") == 0)
			{
				fscanf(archivo, "%s", rs);
				strcpy(RD.nombre, instruc);
				strcpy(RT.nombre, instruc);
				posRS = buscarRegistro(rs, registros);
				RS = registros[posRS];

				lista = insertarInstruccion(lista, index, nroInstruccion, instruc, RD, RS, RT, NO_EXISTE, "");
			}
			else if (strcmp(instruc, "lw") == 0 || strcmp(instruc, "sw") == 0)
			{
				fscanf(archivo, "%s", rd);	
		    	rd[strlen(rd)-1]= '\0';	//quitar coma.
				fscanf(archivo, "%s", rt); // dir.

				posRD = buscarRegistro(rd, registros);
				RD = registros[posRD];

				separarLwOSw(registros, lista, index, nroInstruccion, instruc, RD, rt);
			}
			else
			{
				char* etiqueta;
				etiqueta = strtok(instruc, ":");
				strcpy(RD.nombre, instruc);
				strcpy(RS.nombre, instruc);
				strcpy(RT.nombre, instruc);
				nroInstruccion--;		//no aumenta el nro de instrucción si es una etiqueta.

				lista = insertarInstruccion(lista, index, NO_EXISTE, "", RD, RS, RT, NO_EXISTE, etiqueta); //es label.
			}
			index++;
			nroInstruccion++;
			fscanf(archivo, "%s", instruc);//obtengo una instruccion.
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
		printf("%d != %d\n", registros[RD].valor, registros[RS].valor);
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
/*void ejecutarPrograma(Instruccion* lista, Registro* registros)
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
		
		printf("%d - %s %s %d, %s %d, %s %d, imm:%d, label:%s\n", indice->index, indice->nombre,  
			indice->rd.nombre, indice->rd.valor, indice->rs.nombre, indice->rs.valor, indice->rt.nombre, 
			indice->rt.valor, indice->imm, indice->label);
		indice = indice->sgte;
	}
	printf("\nFIN PROGRAMA.\n\n");
}
*/


Instruccion* realizarSalto(Instruccion* indice, Instruccion* lista)
{
	int i;
	if (strcmp(indice->nombre, "beq")==0 || strcmp(indice->nombre, "bne")==0 || 
		strcmp(indice->nombre, "blt")==0 || strcmp(indice->nombre, "bgt")==0)
	{
		if (indice->rt.valor == 1)		// se salta.
		{
			i = buscarLabel(lista, indice->label);
			indice = buscarIndex(lista, i);		//salta a la instrucción después de la etiqueta.
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

int agregarHazard(int ciclos, int nroI, int tipo, char *nombreI, int indexH)
{
	if (strcmp(nombreI, "NOP")==0)
	{
		//indexH--;
		return indexH;
	}
	hazards[indexH].ciclo = ciclos;
	hazards[indexH].indexInstruccion = nroI;
	hazards[indexH].tipo = tipo;
	hazards[indexH].valido = 1;

	if (tipo ==0 && (strcmp(nombreI, "lw")==0 || strcmp(nombreI, "sw")==0))
		strcpy(hazards[indexH].buffer, "MEM/EXE");	//en el buffer EXE/MEM.
	else if (tipo == 1 && (strcmp(nombreI, "j")==0 || strcmp(nombreI, "jal")==0 || strcmp(nombreI, "jr")==0))
	{
		//hazards[indexH].ciclo = ciclos+1;
		strcpy(hazards[indexH].buffer, "IF/ID");	//en el buffer ID/EXE.			
	}
	else if (tipo == 1 && (strcmp(nombreI, "beq")==0 || strcmp(nombreI, "bne")==0 || 
		strcmp(nombreI, "blt")==0 || strcmp(nombreI, "bgt")==0))
	{
		hazards[indexH].ciclo = ciclos+2;
		strcpy(hazards[indexH].buffer, "ID/EXE");	//en el buffer ID/EXE.
	}
	else
		strcpy(hazards[indexH].buffer, "EXE/MEM");	//en el buffer ID/EXE.	
	return ++indexH;
}

void asignarNOP(Traza *t, int i)
{
	if (t[i].hd != NO_EXISTE)
		hazards[t[i].hd].valido =0;
	t[i].instruccion = NOP;
}

Traza* generarTraza(Instruccion* lista, Registro* registros)
{
	int ciclos=0, indT=0, indexH=0;
	Instruccion* indice = lista;
	Traza *t = (Traza*)malloc(sizeof(Traza)*MAX_INSTRUCCIONES);
	int dosMas=0;

	while (dosMas<2 && indT < MAX_INSTRUCCIONES)
	{
		
		if (indice!=NULL && strcmp(indice->nombre, "")==0) {
			indice = indice->sgte;
			continue;
		}
		if (indice!=NULL)
		{
			t[indT].ciclo = ciclos;
			t[indT].instruccion = indice;
			t[indT].valido = 1;
			t[indT].hd = NO_EXISTE;

			//hazard de control.
			int aux=0;
			if (indT>1)
			{
				//ver hazard
				if (strcmp(t[indT-2].instruccion->rd.nombre, t[indT].instruccion->rs.nombre)==0
					|| strcmp(t[indT-2].instruccion->rd.nombre, t[indT].instruccion->rt.nombre)==0)
				{
					t[indT].hd = indexH;
					indexH = agregarHazard(ciclos, t[indT].instruccion->nroInstruccion, 0, t[indT-2].instruccion->nombre, indexH++);
					if (strcmp(t[indT-2].instruccion->nombre, "lw")==0 || strcmp(t[indT-2].instruccion->nombre, "sw")==0)
						aux=1;
				}
			}
			if (indT>0)
			{
				
				if (aux == 0 && (strcmp(t[indT-1].instruccion->rd.nombre, t[indT].instruccion->rs.nombre)==0
					|| strcmp(t[indT-1].instruccion->rd.nombre, t[indT].instruccion->rt.nombre)==0))
				{
					t[indT].hd = indexH;
					indexH = agregarHazard(ciclos, t[indT].instruccion->nroInstruccion, 0, t[indT-1].instruccion->nombre, indexH++);	
					
				}
			}
			
		}
				
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
					asignarNOP(t, indT-1);
					asignarNOP(t, indT);
					ciclos--;
					t[indT].hd = indexH;
					indexH = agregarHazard(ciclos, t[indT-2].instruccion->nroInstruccion, 1, p->nombre, indexH);							
				}
				else if (strcmp(p->nombre, "j")==0 || strcmp(p->nombre, "jal")==0)
				{
					asignarNOP(t, indT-1);
					asignarNOP(t, indT);
					ciclos--;
					t[indT].valido = 0;

					t[indT].hd = indexH;
					indexH = agregarHazard(ciclos, t[indT-2].instruccion->nroInstruccion, 1, p->nombre, indexH);
				}
				else if (strcmp(p->nombre, "jr")==0)
				{
					asignarNOP(t, indT-1);
					asignarNOP(t, indT);
					ciclos--;
					t[indT].valido = 0;

					t[indT].hd = indexH;
					indexH = agregarHazard(ciclos, t[indT-2].instruccion->nroInstruccion, 1, p->nombre, indexH);		
				}
				else
					printf("ERROR, NO DEBERÍA ENTRAR AQUI.");
			}
			else
			{
				if (t[indT].hd!=NO_EXISTE && t[indT-1].instruccion && (strcmp(t[indT-1].instruccion->nombre, "lw")==0 || strcmp(t[indT-1].instruccion->nombre, "sw")==0))
				{
					asignarNOP(t, indT);
					indT++;
					ciclos++;
					continue;
				}
			}

		}
	
		if (indice == NULL)
		{
			dosMas++;
		}
		else
		{
			indice = indice->sgte;
		}
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
			printf("c:%d - %d - %s\n", t[i].ciclo+1, t[i].instruccion->nroInstruccion+1, t[i].instruccion->nombre);
		}
		i++;
	}
}

void mostrarHazard(Hazard* hazards)
{
	int i=0;
	while (i<MAX_INSTRUCCIONES)
	{
		if (hazards[i].valido == 1)
		{
			printf("c:%d - i:%d - t:%d buffer:%s\n", hazards[i].ciclo+1, hazards[i].indexInstruccion+1, hazards[i].tipo, hazards[i].buffer);
		}
		i++;
	}
}

void guardarHazard(Hazard* hazards)
{
	FILE* salida1;
	salida1 = fopen("HAZARDS.txt", "w");
	int i=0, j=1;
	while (i<MAX_INSTRUCCIONES)
	{
		if (hazards[i].valido == 1)
		{
			if (hazards[i].tipo == 1)//de control.
			{
				fprintf(salida1, "%d- Hazard de control instrucción:%d, CC: %d\n", j,  hazards[i].indexInstruccion+1, hazards[i].ciclo+1);
			}
			else
			{
				fprintf(salida1, "%d- Hazard de datos instrucción:%d, CC: %d\n", j,  hazards[i].indexInstruccion+1, hazards[i].ciclo+1);
			}
			j++;
		}
		i++;
	}
	fclose(salida1);
}


void guardarSolucion(Hazard* hazards)
{
	FILE* salida2;
	salida2 = fopen("DETECCION.txt", "w");
	int i=0, j=1;
	while (i<MAX_INSTRUCCIONES)
	{
		if (hazards[i].valido == 1)
		{
			fprintf(salida2, "%d- Encontrado en buffer: %s\n", j, hazards[i].buffer);
			j++;
		}
		i++;
	}
	fclose(salida2);
}


void guardarRegistros(Registro* registros){
	FILE* salida3;
	salida3 = fopen("REGISTROS.txt", "w");
	int i=0;

	for (i=0; i<29; i++)
	{
		fprintf(salida3, "%s= %d\n", registros[i].nombre, registros[i].valor);
	}
	fprintf(salida3, "$sp= %d\n", Desplazamiento);
	fprintf(salida3, "$fp= %d\n", registros[30].valor);
	fprintf(salida3, "$ra= %ld\n", (long)ra);
	fclose(salida3);
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

	printf("Ingrese nombre del archivo de entrada (ejemplo: Entrada.txt):\n");
	fgets(entrada, TAM_PALABRA, stdin);
	quitarSaltoLinea(entrada);

	lista = leerEntrada(entrada, registros, lista);

	t = generarTraza(lista, registros);

	guardarRegistros(registros);
	guardarSolucion(hazards);
	guardarHazard(hazards);

	printf("\nFin del programa.\n");
	return 0;
}