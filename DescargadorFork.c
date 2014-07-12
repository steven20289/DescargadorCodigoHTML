#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "lista.h"
#include "time.h"
#include "terminal.h"

#define _LARGO 1000

/*Variables Globales*/
Lista* colaLink;
char archivo_temporal[_LARGO];
char* codigo_temporal;


/*Struct*/
typedef struct _Ruta
{
	char ruta[1000];
} Ruta;


/*METODOS*/
void proceso();
void cut_string_n(char* data, int i);
void delete_string(char* codigo, char* delete);
void seek_cut_string(char* codigo, char* destino, char* buscar, int extra);
int seek_string(char* codigo, char* busca);
void generar_nombre(char* link);
void parserLink(char* link,char* sitio);
void descargador_archivos(char *url, char *archivo_temporal);
void extraer_archivo(char *archivo_temporal);
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);


/* E: puntero, size_t,size_t,File
 * S: size_t
 * R: - 
 * A: Funcion para escribir codigo web en archivo.html
 */
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) { // funcion para escribir codigo web en archivo .html
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

/* E: char, char
 * S: 
 * R: - 
 * A: Funcion que descarga un archivo
 */
void descargador_archivos(char *url, char *archivo_temporal){
	
		char archivo[_LARGO];
		strcpy(archivo,direccion_descarga);
		strcat(archivo,archivo_temporal);
			
        CURL *curl;
        CURLcode res;
        FILE *archivoAux;
       
        if(!(curl = curl_easy_init()))
        {
                fprintf(stderr, "El descargador de archivos ha fallado en inicializarse.\n");
                return;
        }
       
        else
        {
                archivoAux = fopen(archivo,"wb");
                curl_easy_setopt(curl, CURLOPT_URL, url);                                              
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);                    
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, archivoAux);
                res = curl_easy_perform(curl);
                fclose(archivoAux);
               
                if(res != CURLE_OK)
                {
                        fprintf(stderr, "El descargador de archivos no ha sido capaz de procesar la solicitud.\nCÃƒÂ³digo: %s\n", curl_easy_strerror(res));
                }
               
                curl_easy_cleanup(curl);
        }
        return;
}
 

/* E: char,
 * S: 
 * R: - 
 * A: Funcion que recupera el codigo de un archivo guardado y ejecuta el parser
 */
void extraer_archivo(char *archivo_temporal)
{
		char archivo[_LARGO];
		strcpy(archivo,direccion_descarga);
		strcat(archivo,archivo_temporal);
		
		//printf("\n%s\n", archivo);
        FILE * archivoAux;
        archivoAux = fopen(archivo, "r");
        if(archivoAux == NULL)
        {
                fprintf(stderr, "Hubo un error al intentar completar la operaciÃƒÂ³n.\nEl archivo generago parece estar corrupto o no existe\n");
        }
       
        else
        {
			char letra;
			int largo = 0;
		   
			while((letra = getc(archivoAux)) != EOF)
			{
					largo++;
			}
		   
			rewind(archivoAux);
		   
			char arch[largo + 1];
			int i;
		   
			for(i = 0; i < largo; i++)
			{
					arch[i] = getc(archivoAux);
			}
		   
			arch[largo] = '\0';
			//printf("%s", arch);		
			
			char sitio[_LARGO];
			char link[_LARGO];
			char link_temporal[_LARGO];
			while(strstr(arch, "href="))
			{
				delete_string(arch,"href=");			
				
					
				if (strstr(arch,">"))
				{
					seek_cut_string(arch,link,">",0);					/*Encuentra el link completo <a href=" ... ">*/
					delete_string(arch,">");							/*Limpia codigo de link*/
								
					if(strstr(link,"\""))								/*Elimina el " o ' del href*/
						delete_string(link,"\"");
					else
						delete_string(link,"'");
					
					if(strstr(link,"\""))			
					{									
						seek_cut_string(link,link_temporal,"\"",-1);
						strcpy(link, link_temporal);
					}
					else if(strstr(link,"'"))	
					{
						seek_cut_string(link,link_temporal,"\"",-1);
						strcpy(link, link_temporal);
					}			
					strcpy(sitio,link);
					
					//printf("Link00: %s\n", link);
					parserLink(sitio, link);
				}		
			}			
        }      
        return;
}




/* E: char nombre-link
 * S: - 
 * R: - 
 * A: Genera un nombre para las paginas, quitando los valores invalidos del sistema de archivos
 */
void generar_nombre(char* link)
{
	int i = 0;
	while(i < strlen(link))
	{
		if((link[i]=='/')||(link[i]=='\\')||(link[i]==':')||(link[i]=='*')||
		(link[i]=='?')||(link[i]=='"')||(link[i]=='<')||(link[i]=='>')||
		(link[i]=='|'))
		{
			link[i] = '_';
		}
		i++;
	}
	strcat(link, ".html");
}

/* E: char, char
 * S: - 
 * R: 
 * A: Toma el link y parsea el sitio, escoge terminaciones y demas
 */	
void parserLink(char* sitio,char* link)
{
	if(seek_string(sitio,"www."))								/*Encuentra el dominio*/
		delete_string(sitio,".");
	else if(seek_string(sitio,"//"))
		delete_string(sitio,"//");								
	
	
	if((seek_string(link,".htm"))||(seek_string(link,".com")))	
	{
		char sitio_t[strlen(sitio)];
		strcpy(sitio_t,sitio);
		seek_cut_string(sitio_t,sitio,".",-1);
		if((!seek_string(link,".css"))&&(!seek_string(link,".png"))&&(!seek_string(link,".jpg"))&&(!seek_string(link,".js"))&&
		(!seek_string(link,".bmp"))&&(!seek_string(link,".php"))&&(!seek_string(link,".asp")))
		{
			
			if((seek_string(link,".htm"))||(seek_string(link,"/"))||(seek_string(link,".com")))		
			{							
				if(seek_string(link,"\""))
				{
					char ink_temporal[strlen(link)];
					strcpy(ink_temporal,link);					
					seek_cut_string(ink_temporal,link,"\"",-1);
				}
				
				
				
				char nombre[_LARGO];		
				if((!buscar_lista_int(link,colaLink))&&(verificar_sitio(sitio, colaLink)))
				{		
					strcpy(nombre,link);
					generar_nombre(nombre);	
										
					insertar_final_lista(link,nombre,sitio,1,colaLink);	
					pid_t pID;
				
					pID = vfork();
					if(pID >= 0) 							// fork was successful
					{
						if(pID == 0) 						// child process
						{
							printf("\n Proceso: %d Start:\n", pID);
							descargador_archivos(link,nombre);	
							double time = end_timer();
							printf("-----------------------------------\n");
							printf("Link: %s \n",link);
							printf("\nTiempo: %f milisegundos\n",time);
							printf("-----------------------------------\n");
							printf("\n Proceso: %d End:\n", pID);
							exit(0);
							
						}
						else //Parent process
						{
							pID = wait(NULL);
							extraer_archivo(nombre);
							
						}
					}
					else // fork failed
					{
						printf(" Fallo el fork...No se pudo crear proceso X_X \n");
						return;
					}									
				}
			}			
		}
	}		
}

/* E: char, int
 * S: - 
 * R: pone \0 al final valido, pero deja sucio los demas
 * A: corta los primeros i digitos de char data
 */	
void cut_string_n(char* data, int i)
{
   int j;
   char temp [strlen(data)];
   for(j=0;j<(strlen(data)-i);j++)
   {
	   temp[j] = data[i + j];	      
   }
   temp[(strlen(data)-i)] = '\0';
   strcpy(data,temp);   
}

/* E: char, char
 * S: deja el resultado en codigo
 * R: -
 * A: busca y elimina de codigo hasta encontrar a char delete
 */	
void delete_string(char* codigo, char* delete)
{
	if(strstr(codigo,delete))
	{
		char data[strlen(codigo)];
		strcpy(data,strstr(codigo,delete));
		cut_string_n(codigo,(strlen(codigo) - strlen(data) + strlen(delete)));
	}
}

/* E: char, char, char,int
 * S: deja el resultado en destino 
 * R: -
 * A: busca buscar en codigo y copia a destino desde el inicio hasta buscar, extra adiciona caracteres a copiar extra
 */	
void seek_cut_string(char* codigo, char* destino, char* buscar, int extra)
{
	if(strstr(codigo,buscar))
	{		
		int largo_busqueda = strlen(strstr(codigo,buscar));
		int largo = strlen(codigo) - largo_busqueda + strlen(buscar) + extra;
		strncpy(destino,codigo,largo);
		destino[largo] = '\0';
	}
}

/* E: char, char,
 * S: int 
 * R: -
 * A: busca el char busca en codigo y retorna un int - bool indicando si esta o no esta presente en el codigo
 */	
int seek_string(char* codigo, char* busca)
{
	int respuesta = 0;
	if(strstr(codigo,busca) != NULL)
		respuesta = 1;
	
	return respuesta;
}

/* E: char,
 * S: 
 * R: - 
 * A: Funcion que lleva el control del programa
 */
void proceso()
{
	terminal();
	/*Preparacion de Variables*/
	char sitio[_LARGO];

	
	if ((colaLink = (Lista *) malloc (sizeof (Lista))) == NULL)
	{
		return;
	}	

    strcpy(sitio,link_d);
	inicializar_lista(colaLink);	
	
	start_timer();
	parserLink(link_d,sitio);		
}



/* E: -
 * S: -
 * R: - 
 * A: Main del programa
 */
int main(void)
{	
	proceso();
	return 0;
}
