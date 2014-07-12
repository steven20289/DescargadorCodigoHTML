#include <time.h>

/*VARIABLES GLOBALES*/
clock_t t_ini, t_fin;

void start_timer();
double end_timer();

/* E: -
 * S: - 
 * R: - 
 * A: Inicia el valor de t_ini, pone el tiempo actual
 */
void start_timer()
{ 
	t_ini = clock();
}
  
/* E: -
 * S: Double 
 * R: - 
 * A: Devuelve el tiempo en milisegundos 
 */  
double end_timer()
{
  t_fin = clock();
  double secs;

  secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;  
  return secs*1000.0;
}
