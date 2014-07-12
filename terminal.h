
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char link_d[10000];
char direccion_descarga[10000];

/* E:-
 * S: char
 * R:
 * A: Obtiene todo lo introducido en la consola*/
char * obtenerLinea(void)
{
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;
 
    if(line == NULL)
        return NULL;
 
    for(;;)
    {
        c = fgetc(stdin);
        if(c == EOF)
            break;
 
        if(--len == 0)
        {
            char * linen = realloc(linep, lenmax *= 2);
            len = lenmax;
 
            if(linen == NULL)
            {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }
 
        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

/* E:-
 * S: 
 * R:
 * A: Terminal del programa, se comparte entre los 3 programas*/
void terminal()
{
	printf("\n Bienvenido al descargador de Sitios\nAutores: Ricardo Chaves - Guianluca Capra\nSistemas Operativos 2013\n");
	printf("\n %s \n","Digite la direccion web:");
	scanf("%s",link_d);
	printf("\n %s \n","Digite la direccion de descarga:");
	scanf("%s",direccion_descarga);
}

