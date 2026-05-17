#ifndef FABRICA_H
#define FABRICA_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE    100

extern char  nombres[MAX_PRODUCTOS][MAX_NOMBRE];
extern int   cantidades[MAX_PRODUCTOS];
extern float tiempos[MAX_PRODUCTOS];
extern float recursos[MAX_PRODUCTOS];
extern int   totalProductos;
extern float tiempoDisponible;
extern float recursosDisponibles;




void  limpiarBuffer(void);
int   solo_Letras(char *nombre);
int   leerEntero(void);        
float leerFlotante(void);      


int  buscarProducto(char nombres[][MAX_NOMBRE], int num_prod,
                    char *terminoBusqueda);
void ingresarProductos(void);
void mostrarProductos(void);
void calcularResultados(int *cant, float *tiempos_p, float *recursos_p,
                        int num_prod, float limit_t, float limit_r);
void editarProducto(void);
void eliminarProducto(char nombres[][MAX_NOMBRE], int *cant,
                      float *tiempos_p, float *recursos_p,
                      int *num_prod, int indice);
void ajustarLimites(void);

#endif 
