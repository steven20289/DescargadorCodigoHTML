#include <stdio.h>
#include <string.h>


typedef struct _Nodo
{
	char link[1000];
	char pagina[1000];
	char sitio[1000];
	int descargada;
	struct _Nodo *siguiente;
} Nodo;

typedef struct _Lista
{
  Nodo *inicio;
  Nodo *fin;
  int tamano;
} Lista;


/* Metodos Lista */
void inicializar_lista(Lista* lista);
int insertar_vacia_lista(char* link, char* pagina, char* sitio,int descargada, Lista *lista);
int insertar_inicio_lista(char* link, char* pagina, char* sitio, int descargada,Lista *lista);
int insertar_final_lista(char* link, char* pagina, char* sitio, int descargada,Lista *lista);
void mostrar_lista(Lista * lista);
Nodo buscar_lista_nodo(char* link,Lista *lista);
int buscar_lista_int(char* link,Lista *lista);
int eliminar_lista(char* pagina,Lista *lista);
int eliminar_inicio_lista(Lista *lista);
int eliminar_pos_lista (char* pagina,Lista *lista);
int verificar_sitio(char* sitio, Lista* lista);


/* *********************************************************************
 * ********************LISTA ELEMENTOS**********************************
 * *********************************************************************/
 
 
void inicializar_lista(Lista* lista)
{
			
	lista ->inicio = NULL;
	lista ->fin = NULL;
	lista ->tamano = 0;
	
	
	
}

/* E: lista - dato
 * S: - 
 * R: -
 * A: insercion en une lista vacia */
int insertar_vacia_lista(char* link, char* pagina, char* sitio,int descargada, Lista *lista)
{
	Nodo *nuevo_elemento;
	
	if ((nuevo_elemento = (Nodo *) malloc (sizeof (Nodo))) == NULL)
	{
		return -1;
	}
	
	/* PASO DE DATOS */	
	strcpy(nuevo_elemento->link,link);	
	strcpy(nuevo_elemento->pagina,pagina);
	strcpy(nuevo_elemento->sitio,sitio);
	nuevo_elemento->descargada=descargada;
	
	
	nuevo_elemento->siguiente = NULL;
	lista->inicio = nuevo_elemento;
	lista->fin = nuevo_elemento;
	lista->tamano++;
	return 0;
}
  
/* E: lista - dato
 * S: - 
 * R: -
 * A: inserta en una lista*/
int insertar_inicio_lista(char* link, char* pagina, char* sitio, int descargada,Lista *lista)
{
	int respuesta;
	if(lista->tamano == 0)
	{
		respuesta = insertar_vacia_lista(link,pagina,sitio, descargada,lista);		
	}
	else
	{
		Nodo *nuevo_elemento;
		if ((nuevo_elemento = (Nodo *) malloc (sizeof (Nodo))) == NULL)
		{
			return -1;
		}
		
		/* PASO DE DATOS */	
		strcpy(nuevo_elemento->link,link);	
		strcpy(nuevo_elemento->pagina,pagina);
		strcpy(nuevo_elemento->sitio,sitio);	
		nuevo_elemento->descargada = descargada;
		
		
		nuevo_elemento->siguiente = lista->inicio;
		lista->inicio = nuevo_elemento;
		lista->tamano++;
		respuesta = 0;	
	}
	return respuesta;
}





/* E: lista - dato
 * S: - 
 * R: -
 * A: insercion al final de la lista */
int insertar_final_lista(char* link, char* pagina, char* sitio, int descargada,Lista *lista)
{
	int respuesta;
	if(lista->tamano == 0)
	{
		respuesta = insertar_vacia_lista(link,pagina,sitio, descargada,lista);		
	}
	else
	{
		
		Nodo *nuevo_elemento;
		if ((nuevo_elemento = (Nodo *) malloc (sizeof (Nodo))) == NULL)
		{
			return -1;
		}
		
		/* PASO DE DATOS */	
		strcpy(nuevo_elemento->link,link);	
		strcpy(nuevo_elemento->pagina,pagina);
		strcpy(nuevo_elemento->sitio,sitio);
		nuevo_elemento->descargada = descargada;
		
		
		lista->fin->siguiente = nuevo_elemento;
		lista->fin = nuevo_elemento;
		lista->tamano++;
		respuesta = 0;
	} 
	return respuesta;
}

/* E: lista 
 * S: Consola
 * R: -
 * A: Imprime los elemento en consola
 */
void mostrar_lista(Lista * lista)
{
	Nodo * nuevo_elemento;
	nuevo_elemento = lista->inicio;
	while(nuevo_elemento != NULL)
	{
		printf("%s\n","**************************");
		printf(" pagina %s\n",nuevo_elemento->pagina);
		printf(" link %s\n",nuevo_elemento->link);
		printf(" sitio %s\n",nuevo_elemento->sitio);
		printf(" Descargada %d\n",nuevo_elemento->descargada);
		printf("%s\n","**************************");
		
		nuevo_elemento = nuevo_elemento->siguiente;
	}
}

/* E: Lista Elemento, Argumento
 * S: Puntero Nodo Plantilla
 * R: -
 * A: Devuelve el puntero del elemento buscado*/
Nodo buscar_lista_nodo(char* link,Lista *lista)
{
	Nodo *nodo;
	nodo = lista->inicio;
	int respuesta = 0;
	while((!respuesta)&&(nodo != NULL))
	{
		if(strcmp(nodo->link,link)==0)
		{
			respuesta++;
		}
		nodo = nodo->siguiente;
		
	}
	
	return *nodo;
}

/* E: Char, Lista
 * S: 1 encontrado/0 no encontrado
 * R: -
 * A: Devuelve resultado bool si encontro o no encontro resultado*/
int buscar_lista_int(char* link,Lista *lista)
{
	Nodo *nodo;
	nodo = lista->inicio;
	int i = 0;
	int respuesta = 0;
	if(lista->tamano != 0)
	{
		while((i < lista->tamano)&&(!respuesta))
		{			
			if(strcmp(nodo->link,link)==0)
			{
				respuesta = 1;
			}
			nodo = nodo->siguiente;
			i++;
			
		}
	}	
	return respuesta;
}


/* E: lista - dato
 * S: - 
 * R: -
 * A: supresión de un elemento de la lista Tablas*/
int eliminar_lista(char* pagina,Lista *lista)
{
	int respuesta = 0;
	if(lista->inicio == NULL)
	{
		respuesta = -1;
	}
	else
	{
		if(strcmp(lista->inicio->pagina,pagina))
		{
			respuesta = eliminar_inicio_lista(lista);
		}
		else
		{
			respuesta = eliminar_pos_lista(pagina, lista);
		}
	}
	return respuesta;
}

/* E: lista - dato
 * S: - 
 * R: -
 * A: supresión al inicio de la lista */
int eliminar_inicio_lista(Lista *lista)
{
	if (lista->tamano == 0)
	{
		return -1;
	}
	Nodo *sup_elemento;
	sup_elemento = lista->inicio;
	lista->inicio = lista->inicio->siguiente;
	
	if (lista->tamano == 1)
	{
		lista->fin = NULL;
	}
	
	//destruirListaFilas(sup_elemento->listaFilas);
	free (sup_elemento);
	lista->tamano--;
	return 0;
}

/* E: lista - dato
 * S: - 
 * R: -
 * A: suprimir un NodoBase después de la posición solicitada */
int eliminar_pos_lista (char* pagina,Lista *lista)
{
	if ((lista->tamano)<= 1 )
	{
	  return -1;
	}
	
	Nodo *temp;
	Nodo *sup_elemento;
	temp = lista->inicio;
	sup_elemento = lista->inicio->siguiente;

	int bool = 1;
	while((bool)&&(sup_elemento != NULL))
	{
		/*encontramos el nodo, moveremos el temp hasta la pocicion anterior para borrarlo*/
		if(strcmp(sup_elemento->pagina,pagina))
		{
			bool--;
			if(temp->siguiente == lista->fin)
			{
				lista->fin = temp;
				lista->fin->siguiente = NULL;
			}
			else
			{
				temp->siguiente = temp->siguiente->siguiente;
			}	
						
			free(sup_elemento);
			lista->tamano--;
		}
		else
		{
			temp = temp->siguiente;
			sup_elemento = sup_elemento->siguiente;
		}
	}
	return 0;
}

int verificar_sitio(char* sitio, Lista* lista)
{
	int respuesta = 0;
	if(lista->tamano != 0)
	{
		if(strcmp(lista->inicio->sitio, sitio) == 0)
			respuesta = 1;
	}
	else
		respuesta = 1;
		
	return respuesta;
}


			
