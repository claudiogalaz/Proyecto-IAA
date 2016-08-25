#ifndef _DATOS_H_INCLUDED_
#define _DATOS_H_INCLUDED_

#define CLIENTES 26

//Información General de la Instancia
typedef struct General
{
	char instancia[10];
	int cantMaxVehiculos;
	int capacidad;
}tipoGeneral;

typedef tipoGeneral *tG;

//Información de cada Cliente
typedef struct Cliente
{
	int num;
	int xCoord;
	int yCoord;
	int demanda;
	int readyTime;
	int dueTime;
	int serviceTime;
}tipoCliente;

typedef struct Cliente *tC;

//Nodo de un elemento de la solución
typedef struct Solucion
{
	int num;
	struct Solucion *sig;
}tipoSolucion;

typedef struct Solucion *Sol;

//guarda los datos de una instancia en las dos estructuras que se le envian
void guardarDatos(tG datosGenerales,tC arregloClientes[CLIENTES]);

//retorna 1 si la solucion satisface la restricción y 0 si no
int revisarCapacidad(Sol solucion,tC arregloClientes[CLIENTES],int capacidad);

//retorna la distancia euclidiana entre dos puntos
double distranciaEuclidiana(int x1, int y1,int x2,int y2);

//retorna 1 si se cumplen las restricciones y 0 si no
int restricciones(Sol solucion,tC aC[CLIENTES],tG datosGenerales);

//devuelve una solucion inicial
Sol greedy(tG datosGenerales,tC aC[CLIENTES]);

int largoSol(Sol solucion);

Sol greedy2(tG datosGenerales,tC aC[CLIENTES], int bla);

#endif