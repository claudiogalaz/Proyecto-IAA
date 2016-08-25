/*
 =====================================================
 Name        : main.c
 Problema	 : CVRPTW
 Instancias	 : Solomon's Benchmark
 Author      : Claudio Galaz
 ROL         : 201073562-1
 Año		 : 2016
 =====================================================
 */



 //CAMBIAR LECTURA DE ARCHIVO!!!!



#include "Librerias/datos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdio>
using namespace std;

#define CANTPOBLACION 100
#define ITERACIONES 7000

//datos.c
FILE *archivo;
Sol poblacion [CANTPOBLACION];
Sol poblacionSgte [CANTPOBLACION];

double maxObj1, minObj1;
int maxObj2, minObj2;
double Obj1[CANTPOBLACION];
int Obj2[CANTPOBLACION];
double normObj1[CANTPOBLACION], normObj2[CANTPOBLACION];
double probFitness[CANTPOBLACION];
double fitness[CANTPOBLACION];
double ruleta[CANTPOBLACION];
int maxVehiculos;
double maxCosto;

float probMutacion = 70.0;

float propElitismo = 0.08;


//double pareto[200][2];

//Clase Pareto para poder hacer el frente de pareto.
class Pareto {
public:
	double obj1;
	int obj2;
	Pareto(double,int);
};

Pareto::Pareto (double a, int b){
	obj1 = a;
	obj2 = b;
}

//Creo un vector que será el frente de pareto...
vector<Pareto> pareto;

void guardarDatos(tG datosGenerales,tC arregloClientes[CLIENTES])
{

	char inst[30];
	char aux[30]="";
	char linea[100];
	int i=0;

	printf("Ingresar nombre del archivo (sin \".txt\"). Ejemplo: C101\n");
	scanf("%s",aux);
	//strcpy(inst,"./solomon_25/C101.txt");
	strcpy(inst, "./solomon_25/");
	strcat(inst, aux);
	strcat(inst, ".txt");

	printf("Datos: %s\n",&inst);

	archivo = fopen(inst,"r");

	if(archivo==NULL)
	{
		printf("archivo %s no encontrado\n",inst);
		return;
	}

	fscanf(archivo,"%s",datosGenerales->instancia);
	//printf("instancia: %s ",datosGenerales->instancia);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%d",&datosGenerales->cantMaxVehiculos);
	fscanf(archivo,"%d",&datosGenerales->capacidad);
	//printf("vehiculos: %d, capadidad: %d\n",datosGenerales->cantMaxVehiculos,datosGenerales->capacidad);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);
	fscanf(archivo,"%s",linea);

	for ( i=0 ; i< (datosGenerales->cantMaxVehiculos+1) ; i++)
	{
		fscanf(archivo,"%d",&arregloClientes[i]->num);
		//printf("%d\n", arregloClientes[i]->num);
		fscanf(archivo,"%d",&arregloClientes[i]->xCoord);
		fscanf(archivo,"%d",&arregloClientes[i]->yCoord);
		fscanf(archivo,"%d",&arregloClientes[i]->demanda);
		fscanf(archivo,"%d",&arregloClientes[i]->readyTime);
		fscanf(archivo,"%d",&arregloClientes[i]->dueTime);
		fscanf(archivo,"%d",&arregloClientes[i]->serviceTime);
	}
	return;
}

//NUEVO GREEDY
Sol greedy(tG datosGenerales,tC aC[])
{
	//printf("Entré al Greedy\n");
	Sol conjunto,aux,aux2,aux3=NULL,solucion=NULL;
	int i,largoConjunto;
	conjunto = (Sol)malloc(sizeof(tipoSolucion));
	conjunto->num=1;
	conjunto->sig=NULL;
	aux=conjunto;

	int r;
	//printf("Generaré el conjunto de clientes\n");
	//genero el conjunto de clientes
	for( i=2 ; i <= datosGenerales->cantMaxVehiculos ; i++)
	{
		aux->sig = (Sol)malloc(sizeof(tipoSolucion));
		aux=aux->sig;
		aux->num=i;
		aux->sig=NULL;
	}
	aux=conjunto;

	largoConjunto = datosGenerales->cantMaxVehiculos;

	//mientras la solución no esté completa
	//printf("Entrando al while\n");
	while(largoSol(conjunto)!=0)
	{
		//caso inicial, solución vacía
		if(solucion==NULL)
		{
			//printf("Entré en el if solucion vacia\n");
			solucion=(Sol)malloc(sizeof(tipoSolucion));
			solucion->num=0;
			solucion->sig=NULL;
			aux2=solucion;
		}
		//solución incompleta
		else
		{
			//printf("Entré en el else de solucion incompleta\n");
			//escojo un cliente aleatorio
			//del conjunto
			if((largoConjunto-1)!=0)
				r = rand()%(largoConjunto);
			else
				r=0;
			//r++;

			//printf("el numero random es: %d\n", r);
			//printf("largo del conjunto es: %d\n", largoSol(conjunto));
			//encuentro el cliente a
			//insertar en la solución
			for( i=1 ; i <= r ; i++)
			{
				aux3=aux;
				aux=aux->sig;
			}
			if(r==0 /*&& largoSol(conjunto)!=1*/)
			{
				aux3=conjunto;
				aux=aux3->sig;
				aux2->sig=aux3;
				conjunto=aux;
				aux3->sig=NULL;
				if(restricciones(solucion,aC,datosGenerales)==0)
				{
					aux2->sig=(Sol)malloc(sizeof(tipoSolucion));
					aux2=aux2->sig;
					aux2->num=0;
					aux2->sig=aux3;
					aux2=aux3;
				}
			}
			else
			{
				//inserto el cliente
				//en la solución
				//quitandolo del conjunto
				
				aux2->sig=aux;
				aux3->sig=aux->sig;
				aux->sig=NULL;
				//en caso de que la solución se vuelva infactible
				//creo una nueva ruta para asignar el cliente
				if(restricciones(solucion,aC,datosGenerales)==0)
				{
					//printf("ENTRÉ EN INFACTIBLE\n");
					aux2->sig=(Sol)malloc(sizeof(tipoSolucion));
					aux2=aux2->sig;
					aux2->num=0;
					aux2->sig=aux;
					aux2=aux;
				}
			}
			//actualizo variables
			while(aux2->sig!=NULL)
				aux2=aux2->sig;
			aux=conjunto;
			largoConjunto--;
		}
	}
	return solucion;
}


int largoSol(Sol solucion)
{
	int count=0;
	Sol aux;
	aux=solucion;
	while(aux!=NULL)
	{
		count++;
		aux=aux->sig;
	}
	return count;
}

// RESTRICCIONES.C

//retorna 1 si cumple y 0 si no
int revisarCapacidad(Sol solucion,tC arregloClientes[CLIENTES],int capacidad)
{
	Sol aux;
	aux=solucion;
	int count,i;
	while(aux!=NULL)
	{
		if(aux->num==0)
			count=0;
		i = aux->num;
		count = count + arregloClientes[i]->demanda;
		aux=aux->sig;
		printf("capacidad ruta: %d\n",count);
		if(count > capacidad)
			return 0;
	}
	return 1;
}

double distranciaEuclidiana(int x1, int y1,int x2,int y2)
{
	return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

//retorna 1 si la solucion cumple todas las restricciones y 0 si no
int restricciones(Sol solucion,tC aC[],tG datosGenerales)
{
	Sol aux;
	aux = solucion;

	int t_max = aC[0]->dueTime;

	double a=0,w=0,t=0,t_acum=0;	//a = arrival time | w = waiting time | t = time between nodes | t_acum = tiempo acumulado

	int e=0,l=0,q=0,s=0,pc=0;			//e = ready time | l = due time | q = demanda | s = service time | pc = previous costumer

	while(aux!=NULL)
	{
		if(aux->num==0)
		{
			a=w=q=0;
			s=aC[0]->serviceTime;
			pc=0;
			t=distranciaEuclidiana(aC[pc]->xCoord,aC[pc]->yCoord,aC[aux->num]->xCoord,aC[aux->num]->yCoord);
			//restricción de tiempo máximo
			if( t+t_acum > t_max)
				return 0;
			t_acum=0;
			//aux=aux->sig;
		}
		else
		{
			//tiempo de viaje
			t=distranciaEuclidiana(aC[pc]->xCoord,aC[pc]->yCoord,aC[aux->num]->xCoord,aC[aux->num]->yCoord);
			//tiempo de llegada
			a=a+w+s+t;
			e=aC[aux->num]->readyTime;
			l=aC[aux->num]->dueTime;
			s=aC[aux->num]->serviceTime;
			q=q+aC[aux->num]->demanda;
			//llegar a tiempo
			if( (double)e <= a && a <= (double)l )
				w=0;
			//llegar antes
			else if( a < (double)e ){
				w=(double)e-a;
				//printf("EL w VALE: %d\n", );
			}
			//llegar tarde
			else
				return 0;
			//fuera de la ventana
			if(!((double)e <= (a+w) && (a+w) <= (double)l))
				return 0;
			//capacidad max vehiculos
			if( q > datosGenerales->capacidad )
				return 0;
			t_acum=t+(double)s+w;
			//tiempo máximo
			if(t_acum > t_max)
				return 0;
			pc=aux->num;
			//aux=aux->sig;
		}
		aux=aux->sig;
	}
	return 1;
}



Sol greedy2(tG datosGenerales,tC aC[CLIENTES],int bla)
{
	//printf("Entré al Greedy\n");
	Sol conjunto,aux,aux2,aux3=NULL,solucion=NULL;
	int i,largoConjunto;
	conjunto = (Sol)malloc(sizeof(tipoSolucion));
	conjunto->num=1;
	conjunto->sig=NULL;
	aux=conjunto;
	int r;
	//printf("Generaré el conjunto de clientes\n");
	//genero el conjunto de clientes
	for( i=2 ; i <= CLIENTES-1 /*datosGenerales->cantMaxVehiculos*/ ; i++)
	{
		aux->sig = (Sol)malloc(sizeof(tipoSolucion));
		aux=aux->sig;
		aux->num=i;
		aux->sig=NULL;
	}
	aux=conjunto;

	largoConjunto = CLIENTES-1; //datosGenerales->cantMaxVehiculos;

	//mientras la solución no esté completa
	//printf("Entrando al while\n");
	while(largoSol(conjunto)!=0)
	{
		//caso inicial, solución vacía
		if(solucion==NULL)
		{
			//printf("Entré en el if solucion vacia\n");
			solucion=(Sol)malloc(sizeof(tipoSolucion));
			solucion->num=0;
			solucion->sig=NULL;
			aux2=solucion;
		}
		//solución incompleta
		else
		{
			//printf("Entré en el else de solucion incompleta\n");
			//escojo un cliente aleatorio
			//del conjunto


			//srand(time(NULL)*(bla+1));


			if((largoConjunto-1)!=0)
				r = rand()%(largoConjunto-1);
			else
				r=0;
			//r++;

			//printf("el numero random es: %d\n", r);
			//printf("largo del conjunto es: %d\n", largoSol(conjunto));
			//encuentro el cliente a
			//insertar en la solución
			for( i=1 ; i <= r ; i++)
			{
				if(i==r)
					aux3=aux;
				aux=aux->sig;
			}
			if(r==0 /*&& largoSol(conjunto)!=1*/)
			{
				aux3=conjunto;
				aux=aux3->sig;
				aux2->sig=aux3;
				conjunto=aux;
				aux3->sig=NULL;
				if(restricciones(solucion,aC,datosGenerales)==0)
				{
					aux2->sig=(Sol)malloc(sizeof(tipoSolucion));
					aux2=aux2->sig;
					aux2->num=0;
					aux2->sig=aux3;
					aux2=aux3;
				}
			}
			else
			{
				//inserto el cliente
				//en la solución
				//quitandolo del conjunto
				
				aux2->sig=aux;
				aux3->sig=aux->sig;
				aux->sig=NULL;
				//en caso de que la solución se vuelva infactible
				//creo una nueva ruta para asignar el cliente
				if(restricciones(solucion,aC,datosGenerales)==0)
				{
					aux2->sig=(Sol)malloc(sizeof(tipoSolucion));
					aux2=aux2->sig;
					aux2->num=0;
					aux2->sig=aux;
					aux2=aux;
				}
			}
			//actualizo variables
			while(aux2->sig!=NULL)
				aux2=aux2->sig;
			aux=conjunto;
			largoConjunto--;
		}
	}
	return solucion;
}

//Funcion Objetivo 1
double funcObjetivo_Costo(tG datosGenerales, tC arregloClientes[CLIENTES], Sol solu){
	Sol aux, sig;
	tC cliente, clienteSgte;
	aux = solu;
	double costo;
	costo = 0;
	int clienteNum;

	//RECORDAR!!! Se deben sumar además los costos de vuelta desde la ciudad final hasta el cero
	//EDIT: Hay que sumar solo el último cero que falta, que es la vuelta de la última ruta.
	//EDIT2: Arreglado XD

	while (aux != NULL){
		cliente = arregloClientes[aux->num];
		clienteNum=aux->num;
		sig = aux->sig;
		if (sig == NULL && clienteNum != 0){
			clienteSgte = arregloClientes[0];
		}
		else if (sig != NULL){
			clienteSgte = arregloClientes[sig->num];
		}
		costo = costo + distranciaEuclidiana(cliente->xCoord, cliente->yCoord, clienteSgte->xCoord, clienteSgte->yCoord);
		aux = aux->sig;
	}
	//printf("COSTO TOTAL: %f\n", costo);
	return costo;
}

//Funcion Objetivo 2
int funcObjetivo_Vehiculos(Sol solu){
	int vehiculos, i;
	Sol aux;
	vehiculos = 0;
	i=0;
	aux = solu;

	while (aux != NULL){
		if(aux->num == 0 && aux->sig == NULL){
			//vehiculos = vehiculos - 1;
			//printf("HOLAAA!!!\n");
			break;
		}
		if (aux->num == 0){
			vehiculos = vehiculos + 1;
			i++;
		}
		aux = aux->sig;

	}
	//printf("vehiculos: %d\n", vehiculos);
	return vehiculos;
}


void creaPoblacion(tG datosGenerales,tC arregloClientes[CLIENTES]){
	int i;
	Sol aux;
	for (i = 0; i < CANTPOBLACION; i++){
		poblacion[i] = greedy2(datosGenerales, arregloClientes, i);
		aux = poblacion[i];
		//Imprime poblacion inicial
		while(aux != NULL)
		{
			//printf("%d -> ", aux->num);
			aux = aux->sig;
		}
		//printf("\n");
		//funcObjetivo_Costo(datosGenerales, arregloClientes, poblacion[i]);
		//funcObjetivo_Vehiculos(poblacion[i]);
	}
	return;
}

float ponderacion(int i){
	int r;
	float ponderacion;

	//srand(time(NULL)*1);
	r = rand()%100 + 1;
	while((1 < r && r < 6) || (90 < r && r <= 100)){
		r = rand()%100 + 1;
	}
	ponderacion = (float)(r)/100;
	return ponderacion;
}

void calcProbabilidad(){
	int i;
	double sumaFitness;
	sumaFitness = 0;

	for (i = 0; i < CANTPOBLACION; i++){
		sumaFitness = sumaFitness + ((double)(1)/(double)(fitness[i]));
	}

	for (i = 0; i < CANTPOBLACION; ++i){
		probFitness[i] = (1/fitness[i])/(sumaFitness);
	}

	for (i = 0; i < CANTPOBLACION; i++){
		if(i == 0)
			ruleta[i] = probFitness[i];
		else
			ruleta[i] = ruleta[i-1] + probFitness[i];
	}
	return;
}

void eliminaElementos(Sol padre1, int pila1[], int size1, Sol padre2, int pila2[], int size2){
	int i,j;

	Sol aux1, aux2;
	Sol nuevoPadre1, nuevoPadre2;
	Sol auxNuevo1, auxNuevo2;

	aux1 = padre1;
	aux2 = padre2;

	nuevoPadre1 = (Sol)malloc(sizeof(tipoSolucion));
	auxNuevo1 = nuevoPadre1;
	while(aux1 != NULL){
		i = 0;
		for (j = 0; j < size1; j++)
		{
			if(aux1->num == pila1[j])
				i = aux1->num;
		}
		if(i == 0){
			auxNuevo1->num = aux1->num;
			auxNuevo1 = auxNuevo1->sig;
		}
		aux1 = aux1->sig;
		if(aux1 == NULL)
			auxNuevo1->sig = NULL;
	}

	nuevoPadre2 = (Sol)malloc(sizeof(tipoSolucion));
	auxNuevo2 = nuevoPadre2;
	while(aux2 != NULL){
		i = 0;
		for (j = 0; j < size2; j++)
		{
			if(aux2->num == pila2[j])
				i = aux2->num;
		}
		if(i == 0){
			auxNuevo2->num = aux2->num;
			auxNuevo2 = auxNuevo2->sig;
		}
		aux2 = aux2->sig;
		if(aux2 == NULL)
			auxNuevo2->sig = NULL;
	}
}

int cruzamiento(Sol padre1, Sol padre2, int bla, tG dG, tC aC[CLIENTES]){
	int cantRutasPadre1, cantRutasPadre2;
	int r1,r2,i1,i2;
	Sol aux1, aux2;
	int pila1[100],pila2[100];

	int n, j, k; //Variables auxiliares
	n=0;
	j=0;

	i1 = 0;
	i2 = 0;
	
	cantRutasPadre1 = funcObjetivo_Vehiculos(padre1);
	cantRutasPadre2 = funcObjetivo_Vehiculos(padre2);

	//srand(time(NULL)*(bla+1));
	r1 = rand()%cantRutasPadre1 + 1;
	//srand(time(NULL)*(bla+1)*2);
	r2 = rand()%cantRutasPadre2 + 1;
	aux1 = padre1;
	while(aux1 != NULL){
		if(aux1->num == 0)
			i1 = i1 + 1;
		if(i1 == r1)
			break;
		aux1 = aux1->sig;
	}
	while(aux1 != NULL){
		if(aux1->num == 0 && j != 0){
			break;
		}
		if(aux1->num != 0){
			pila1[j] = aux1->num;
			j++;
		}
		aux1 = aux1->sig;
	}

	//seteo parametros
	aux2 = padre2;
	while(aux2 != NULL){
		if(aux2->num == 0)
			i2 = i2 + 1;
		if(i2 == r2)
			break;
		aux2 = aux2->sig;
	}
	while(aux2 != NULL){
		if(aux2->num == 0 && n != 0){
			break;
		}
		if(aux2->num != 0){
			pila2[n] = aux2->num;
			n++;
		}
		aux2 = aux2->sig;
	}


	//ELIMINA ELEMENTOS  -----  CREA PADRES SIN ELEMENTOS
	int i, size1, size2;
	size1 = j;
	size2 = n;

	Sol nuevoPadre1, nuevoPadre2;
	Sol auxNuevo1, auxNuevo2, elimina;

	aux1 = padre1;
	aux2 = padre2;

	nuevoPadre1 = (Sol)malloc(sizeof(tipoSolucion));
	auxNuevo1 = nuevoPadre1;
	while(aux1 != NULL){
		i = 0;
		for (j = 0; j < size2; j++)
		{
			if(aux1->num == pila2[j])
				i = aux1->num;
		}
		if(i == 0){
			auxNuevo1->num = aux1->num;
			auxNuevo1->sig = (Sol)malloc(sizeof(tipoSolucion));
			auxNuevo1 = auxNuevo1->sig;
		}
		aux1 = aux1->sig;
		if(aux1 == NULL){
			auxNuevo1->sig = NULL;
		}
			
	}

	nuevoPadre2 = (Sol)malloc(sizeof(tipoSolucion));
	auxNuevo2 = nuevoPadre2;
	while(aux2 != NULL){
		i = 0;
		for (j = 0; j < size1; j++)
		{
			if(aux2->num == pila1[j])
				i = aux2->num;
		}
		if(i == 0){
			auxNuevo2->num = aux2->num;
			auxNuevo2->sig = (Sol)malloc(sizeof(tipoSolucion));
			auxNuevo2 = auxNuevo2->sig;
		}
		aux2 = aux2->sig;
		if(aux2 == NULL)
			auxNuevo2->sig = NULL;
	}

	Sol nuevo_padre1, nuevo_padre2;

	aux1 = nuevoPadre1;
	//ELIMINAR LOS CEROS QUE QUEDEN JUNTOS
	nuevo_padre1 = (Sol)malloc(sizeof(tipoSolucion));
	auxNuevo1 = nuevo_padre1;
	while(aux1 != NULL){
		i=0;
		if(aux1->num == 0)
			i++;
		auxNuevo1->num = aux1->num;
		auxNuevo1->sig = (Sol)malloc(sizeof(tipoSolucion));
		elimina = aux1;
		aux1 = aux1->sig;
		free(elimina);
		if(aux1 == NULL){
			auxNuevo1->sig = NULL;
			break;
		}
		if(aux1 != NULL && aux1->num == 0 && i == 1){
		}
		else
			auxNuevo1 = auxNuevo1->sig;
	}

	aux2 = nuevoPadre2;
	nuevo_padre2 = (Sol)malloc(sizeof(tipoSolucion));
	auxNuevo2 = nuevo_padre2;
	while(aux2 != NULL){
		i=0;
		if(aux2->num == 0)
			i++;
		auxNuevo2->num = aux2->num;
		auxNuevo2->sig = (Sol)malloc(sizeof(tipoSolucion));
		elimina = aux2;
		aux2 = aux2->sig;
		free(elimina);
		elimina = NULL;
		if(aux2 == NULL){
			auxNuevo2->sig = NULL;
			break;
		}
		if(aux2 != NULL && aux2->num == 0 && i == 1){
		}
		else
			auxNuevo2 = auxNuevo2->sig;
	}

	//Ahora hay que usar nuevo_padre1 y nuevo_padre2 para insertarle los elementos que faltan...
	i=0;
	j=0;
	k=0;
	Sol hijo1, hijo2;

	hijo1 = (Sol)malloc(sizeof(tipoSolucion));

	auxNuevo1 = hijo1;
	aux1 = nuevo_padre1;

	while(aux1 != NULL){
		auxNuevo1->num = aux1->num;
		auxNuevo1->sig = (Sol)malloc(sizeof(tipoSolucion));
		elimina = aux1;
		aux1 = aux1->sig;
		free(elimina);
		elimina = NULL;
		if(aux1 == NULL)
		{
			auxNuevo1 = auxNuevo1->sig;

			for(i = 0; i < size2; i++){
				auxNuevo1->num = pila2[i];
				auxNuevo1->sig = (Sol)malloc(sizeof(tipoSolucion));
				auxNuevo1 = auxNuevo1->sig;
				
			}
			auxNuevo1->sig = NULL;
			break;
		}
		auxNuevo1 = auxNuevo1->sig;
	}

	hijo2 = (Sol)malloc(sizeof(tipoSolucion));
	auxNuevo2 = hijo2;
	aux2 = nuevo_padre2;

	while(aux2 != NULL){
		auxNuevo2->num = aux2->num;
		auxNuevo2->sig = (Sol)malloc(sizeof(tipoSolucion));
		elimina = aux2;
		aux2 = aux2->sig;
		free(elimina);
		if(aux2 == NULL)
		{
			auxNuevo2 = auxNuevo2->sig;

			for(i = 0; i < size1; i++){
				auxNuevo2->num = pila1[i];
				auxNuevo2->sig = (Sol)malloc(sizeof(tipoSolucion));
				auxNuevo2 = auxNuevo2->sig;
				
			}
			auxNuevo2->sig = NULL;
			break;
		}
		auxNuevo2 = auxNuevo2->sig;
	}																											//PASA EL MEJOR HIJO
	/*if(funcObjetivo_Costo(dG,aC,hijo1) < funcObjetivo_Costo(dG,aC,hijo2) && restricciones(hijo1,aC,dG) == 1){
		poblacionSgte[bla] = hijo1;
		return 1;
	}
	if(funcObjetivo_Costo(dG,aC,hijo1) >= funcObjetivo_Costo(dG,aC,hijo2) && restricciones(hijo2,aC,dG) == 1){
		poblacionSgte[bla] = hijo2;
		return 1;
	}*/
	
	/*aux1 = poblacionSgte[bla];
	aux2 = poblacionSgte[bla+1];


	while(aux1 != NULL){
		elimina = aux1;
		aux1 = aux1->sig;
		free(elimina);
		elimina = NULL;
	}

	while(aux2 != NULL){
		elimina = aux2;
		aux2 = aux2->sig;
		free(elimina);
		elimina = NULL;
	}*/

	poblacionSgte[bla] = hijo1;
	poblacionSgte[bla+1] = hijo2;

	return 0;

}

void mutacion(tG dG, tC aC[CLIENTES]){
	int i, j, cont, gen1, gen2, gen_1, gen_2;

	int intento, INTENTOS;
	intento = 0;

	INTENTOS = 20;

	float prob;
	Sol aux, prueba;
	Sol nueva, nuevoMutado;
	gen2 = 0;
	gen1 = 0;


	for (i = 0; i < 100; i++)
	{
		intento = 0;
		j=0;
		cont = 0;
		//srand(time(NULL)*(i+1));
		prob = rand()%100;
		//printf("PROBABILIDAD %f\n", prob);
		if(i >= (CANTPOBLACION*propElitismo+1)){ //i !=0 && i != 1 && i != 2 && i != 3){
			
			if (prob <= probMutacion)
			{
				while (intento < INTENTOS){ //20
					cont=0;
					j=0;
					nuevoMutado = (Sol)malloc(sizeof(tipoSolucion));
					//printf("muta %d\n", i);
					aux = poblacionSgte[i];
					//printf("1\n");
					while (aux != NULL){
						cont = cont + 1;
						aux = aux->sig;
					}
					//printf("2\n");
					//printf("INTENTO %d\n", intento);
					gen1 = rand()%(cont-2)+2;
					gen2 = rand()%(cont-2)+2;
					while (gen1 == gen2){
						//srand(time(NULL)*(i+1)*ejecucion);
						gen1 = rand()%(cont-2)+2;
						//printf("gen 1 %d\n", gen1);
						//srand(time(NULL)*(i+5)*ejecucion);
						gen2 = rand()%(cont-2)+2;
						//printf("gen 2 %d\n", gen2);
					}
					//printf("3\n");
					aux = poblacionSgte[i];
					while (aux != NULL){
						j = j + 1;
						if (j == gen1){
							if(aux->num != 0){
								gen_1 = aux->num;
							}
								
							else if(aux->sig != NULL)
								gen_1 = (aux->sig)->num;
						}
						if (j == gen2){
							if(aux->num != 0)
								gen_2 = aux->num;
							else if(aux->sig != NULL)
								gen_2 = (aux->sig)->num;
						}
							
						aux = aux->sig;
					}
					j=0;
					nueva = nuevoMutado;
					aux = poblacionSgte[i];
					//printf("gen1 %d gen2 %d,,,, gen_1 %d gen_2 %d\n", gen1, gen2, gen_1, gen_2);
					while (aux != NULL){
						j = j + 1;
						if(j == gen1)
							nueva->num = gen_2;
						else if(j == gen2)
							nueva->num = gen_1;
						else{
							nueva->num = aux->num;
						}
							
						nueva->sig = (Sol)malloc(sizeof(tipoSolucion));
						if (aux->sig == NULL)
							nueva->sig = NULL;
						nueva = nueva->sig;
						aux = aux->sig;
					}
					if (restricciones(nuevoMutado, aC, dG) == 1)
					{
						/*printf("ANTERIOR: \n");
						prueba = poblacionSgte[i];
						while(prueba!=NULL)
						{
							printf("%d -> ", prueba->num);
							prueba=prueba->sig;
						}
						printf("\n");*/


						//free(poblacionSgte[i]);
						poblacionSgte[i] = nuevoMutado;
						intento = 100;

						/*printf("NUEVO: \n");
						prueba = poblacionSgte[i];
						while(prueba!=NULL)
						{
							printf("%d -> ", prueba->num);
							prueba=prueba->sig;
						}
						printf("\n");
						//printf("MUTO!!!!!\n");*/
					}
					else{
						intento++;
						//free(nuevoMutado);
						//printf("NO MUTA %d\n", intento);
						/*if (intento == 20)
							printf("NO MUTO NUNCA\n");*/
					}
				}
			}
		}
		gen1 = 0;
		gen2 = 0;
	}
}

//Funcion que habia hecho antes, muy similar a la de abajo...

/*void pareto_frontier(tG dG, tC aC[CLIENTES]){

	Sol aux;

	int largo_frente;
	int j;
	double distancia;
	int vehiculos;
	int agregar; //Sirve para saber si se agrega o no al frente

	for (int i = 0; i < CANTPOBLACION; ++i)
	{
		agregar = 1;
		aux = poblacion[i];
		distancia = funcObjetivo_Costo(dG, aC, aux);
		vehiculos = funcObjetivo_Vehiculos(aux);
		largo_frente = pareto.size();

		for (j = 0; j < pareto.size(); ++j)
		{
			if (pareto[j].obj1 == distancia && pareto[j].obj2 == vehiculos){
				agregar = 0;
				break;
			}
			else if((pareto[j].obj1 >= distancia && pareto[j].obj2 > vehiculos) || (pareto[j].obj1 > distancia && pareto[j].obj2 >= vehiculos)){
				printf("elimina %lf por %lf y %d por %d\n", pareto[j].obj1, distancia, pareto[j].obj2, vehiculos);
				pareto.erase(pareto.begin() + j);
				j--;
				//largo_frente = largo_frente - 1;
			}
			else if ((pareto[j].obj1 < distancia && pareto[j].obj2 > vehiculos) || (pareto[j].obj1 > distancia && pareto[j].obj2 < vehiculos))
			{
				printf("ESCRIBE, %lf por %lf y %d por %d\n\n\n\n", distancia,  pareto[j].obj1, vehiculos, pareto[j].obj2);
			}
			else if((pareto[j].obj1 <= distancia && pareto[j].obj2 < vehiculos) || (pareto[j].obj1 < distancia && pareto[j].obj2 <= vehiculos)){
				//Hay que descartar la solución
				//printf("DESCARTA %lf por %lf y %d por %d\n", distancia, pareto[j].obj1, vehiculos, pareto[j].obj2);
				agregar = 0;
				break;
			}

			//printf("LARGO %d\n", pareto.size());

		}
		if(j == largo_frente && agregar == 1){
			if (pareto.size() == 0)
			{
				printf("BLA\n");
			}
			Pareto par (distancia, vehiculos);
			pareto.push_back(par);
			printf("lo mete\n");
		}
		printf("PARETO: \n");
		for (j = 0; j < pareto.size(); ++j)
		{
			printf("obj 1: %lf, obj 2: %d\n", pareto[j].obj1, pareto[j].obj2);
		}
	}

}*/

void pareto_frontier(tG dG, tC aC[CLIENTES]){
	
	Sol aux;
	int i,j,agrega;
	vector<int> borrar;
	
	int vehiculos;
	double distancia;
	int encuentra;


	for (i = 0; i < CANTPOBLACION; i++)
	{
		agrega = 1;
		encuentra = 2;
		aux = poblacion[i];
		distancia = funcObjetivo_Costo(dG,aC,aux);
		vehiculos = funcObjetivo_Vehiculos(aux);

		for (j = 0; j < pareto.size(); j++)
		{
			encuentra = 1;
			if( pareto[j].obj1 == distancia && pareto[j].obj2 == vehiculos){
				agrega = 0;
				encuentra = 0;
				break;
			}
			else if (pareto[j].obj1 < distancia && pareto[j].obj2 <= vehiculos){
				agrega = 0;
				encuentra = 0;
				break;
			}
			else if (pareto[j].obj1 > distancia && pareto[j].obj2 >= vehiculos){
				encuentra = 0;
				pareto.erase(pareto.begin()+j);
				j--;
				//borrar.push_back(j);
			}
		}
		if(encuentra == 1)
			printf("\n\nNUNCA FUE NI MEJOR NI PEOR\n\n\n");

		//Si agrega == 0, es decir, si el individuo a ingresar es igual a uno que ya existe o si es de peor calidad, no ingresa al frente de Pareto.
		if(agrega == 1){
			Pareto par (distancia, vehiculos);
			pareto.push_back(par);
			/*for (j = 0; j < borrar.size(); j++)
			{
				pareto.erase(pareto.begin() + borrar[j]);
			}
			borrar.clear();*/
		}
	}
	printf("FRENTE DE PARETO: \n");
	for (j = 0; j < pareto.size(); ++j)
	{
		printf("objetivo 1: %lf, objetivo 2: %d\n", pareto[j].obj1, pareto[j].obj2);
	}
	return;
}



void genetico(tG datosGenerales, tC arregloClientes[CLIENTES], int ejecucion){
	double prob;
	int i;
	int SUMA;
	double suma_obj1, mayorObj1;
	int suma_obj2, mayorObj2;
	float pond1, pond2;
	mayorObj1 = 0;
	mayorObj2 = 0;
	SUMA = 0;
	/*
	TODO: 
	-Realizar selección con el metodo de ponderar fitness (con ruleta creo).
	-Realizar cruzamiento.
	Con esto, se crea toda la siguiente población, y hay que mutar aquella población.
	Finalmente se puede desechar la población anterior, quedándose con la que se obtiene de la mutación.
	Repetir.
	*/

	//Hay que normalizar los objetivos, con el fin de luego ponderarlos.
	//Primero guardamos los costos de las soluciones
	for (i = 0; i < CANTPOBLACION; i++)
	{
		Obj1[i] = funcObjetivo_Costo(datosGenerales, arregloClientes, poblacion[i]);
	}
	for (i = 0; i < CANTPOBLACION; i++)
	{
		normObj1[i] = Obj1[i]/maxCosto; //SE NORMALIZA OBJETIVO 1
	}
	//Lo mismo pero para la func. objetivo 2
	for (i = 0; i < CANTPOBLACION; i++)
	{
		Obj2[i] = funcObjetivo_Vehiculos(poblacion[i]);
	}
	for (i = 0; i < CANTPOBLACION; i++)
	{
		//printf("obj2 %d, mayor %d, obj2/mayor=%lf // %e\n", Obj2[i], mayorObj2, Obj2[i]/mayorObj2, Obj2[i]/mayorObj2);
		normObj2[i] = (double)(Obj2[i])/(double)(maxVehiculos);
	}

	//Calcula las ponderaciones necesarias para luego calcular fitness
	pond1 = ponderacion(ejecucion);
	pond2 = 1.0 - pond1;
	//printf("EJEC nro. %d, pond1: %f, pond2: %f\n", ejecucion, pond1, pond2);


	//ACORDARSE DE CAMBIAR ESTO DE ABAJO

	//pond1 = 0.75;
	//pond2 = 0.25;
	//printf("pond1: %f, pond2: %f\n", pond1, pond2);



	
	//Hay que convertir estas variables double a float.
	/*float normObj1F, normObj2F;
	normObj1F = (float)(normObj1);
	normObj2F = (float)(normObj2);*/
	
	/*Ahora hay que calcular el fitness*/
	for (i = 0; i < CANTPOBLACION; i++)
	{
		//printf("obj1:%lf,obj2:%lf\n", normObj1[i],normObj2[i]);
		fitness[i] = (double)(pond1)*normObj1[i] + (double)(pond2)*normObj2[i];
		//printf("obj 2 indiv %d normalizado: %lf\n", i,  normObj2[i]);
		//printf("APORTE indiv. %d OBJ 1: %lf\n", i, (double)(pond1)*normObj1[i]);
		//printf("APORTE indiv. %d OBJ 2: %lf\n", i, (double)(pond2)*normObj2[i]);
		//printf("Fitness indiv. %d: %lf\n", i, fitness[i]);
	}

	//Comienza proceso de elitismo. De los 100 individuos, pasan los 5 mejores a la siguiente población
	double menor, menor_previo;
	int pos_nuevo;
	int k;
	k = 1000;
	pos_nuevo = 0;
	menor = 1.0;
	menor_previo = 2.0;
	/*taboo[0] = 101;
	taboo[1] = 101;
	taboo[2] = 101;
	taboo[3] = 101;*/

	/*while (pos_nuevo < 4){
		for (i = CANTPOBLACION-1; i >= 0; i--)
		{
			if(fitness[i] < menor && taboo[0] != i && taboo[1] != i && taboo[2] != i && taboo[3] != i){
				menor = fitness[i];
				k = i;
			}
		}
		//printf("PASA FITNESS: %lf, menor: %lf, k=%d, pos_nuevo=%d\n", fitness[k], menor, k, pos_nuevo);
		poblacionSgte[pos_nuevo] = poblacion[k];
		taboo[pos_nuevo] = k;
		menor = 2.0;
		pos_nuevo++;
	}*/


	int j = 0;
	while(j == 0){
		menor = 1.0;
		for (i = CANTPOBLACION-1; i >= 0; i--){
			if(fitness[i] < menor && menor != menor_previo)
				menor = fitness[i];
		}
		menor_previo = menor;

		//i = (CANTPOBLACION - 1);
		for (i = CANTPOBLACION - 1; i >= 0; i--){
			if(fitness[i] == menor){
				poblacionSgte[pos_nuevo] = poblacion[i];
				pos_nuevo++;
			}
			//printf("pos nuevo %d\n", pos_nuevo);
			if(pos_nuevo >= ((CANTPOBLACION*propElitismo)+1))
				break;
		}
		if(pos_nuevo >= ((CANTPOBLACION*propElitismo)+1))
				break;
	}


	//Quedan 95 individuos, vendrán producto de cruzamiento. Se utilizan probabilidades en base a su calidad.
	calcProbabilidad();

	float r;
	Sol padre[2];
	//srand(time(NULL)*888);
	//Proceso de selección de padres:
	for (i = pos_nuevo; i < CANTPOBLACION; i++){
		//srand(time(NULL)*i*888);
		SUMA = 0;
		//while(SUMA == 0){
			for (k = 0; k < 2; k++) //for ya que son dos padres
			{
				
				r = rand()%100001;
				prob = (double)(r)/(double)(100000);
				//printf("EJEC %d, hijo: %d, r=%f, prob: %lf\n",ejecucion, i, r, prob);
				for (j = 0; j < CANTPOBLACION; j++)
				{
					//printf("RULETA: %lf, PROB: %lf\n",ruleta[0], prob );
					if(ruleta[0] >= prob){
						padre[k] = poblacion[j];
						//printf("fitness Padre %d: %lf\n", k,fitness[j]);
						break;
					}
					
					if(j != 0)
						if(ruleta[j-1] < prob && ruleta[j] >= prob){
							padre[k] = poblacion[j];
							//printf("fitness Padre %d: %lf\n", k,fitness[j]);
							break;
						}
				}
				
				//CREAR SIGUIENTE FUNCION:
				
			}
			cruzamiento(padre[0], padre[1], i, datosGenerales, arregloClientes);
			i++;
		//}
	}
	//printf("bla\n");
	//Mutacion
	mutacion(datosGenerales, arregloClientes);
	//printf("ble\n");

	for (i = 0; i < CANTPOBLACION; i++)
	{
		poblacion[i] = poblacionSgte[i];
	}

	//Revisa el frente de Pareto
	pareto_frontier(datosGenerales, arregloClientes);



	return;
}



int main(int argc,char *argv[])
{

	clock_t start;
    double duration;

    start = clock();

    
	srand(time(NULL));
	printf("%f\n", CANTPOBLACION*propElitismo);
	tG datosGenerales = (tG)malloc(sizeof(tipoGeneral));
	int i=0;
	tC arregloClientes[CLIENTES];
	for (i=0 ; i<(CLIENTES+1) ; i++)
	{
		arregloClientes[i]=(tC)malloc(sizeof(tipoCliente));
	}

	//LLAMAR A "genetico" USANDO EL PARAMETRO "ejecucion" YA QUE determina la iteracion actual
	guardarDatos(datosGenerales,arregloClientes);
	creaPoblacion (datosGenerales,arregloClientes);
	pareto_frontier(datosGenerales, arregloClientes);

	maxVehiculos = 0;
	maxCosto = 0;
	
	for (i = 0; i < CANTPOBLACION; i++)
	{
		Obj1[i] = funcObjetivo_Costo(datosGenerales, arregloClientes, poblacion[i]);
		if(Obj1[i] > maxCosto)
			maxCosto = Obj1[i];
	}
	
	for (i = 0; i < CANTPOBLACION; i++)
	{
		Obj2[i] = funcObjetivo_Vehiculos(poblacion[i]);
		if(Obj2[i] > maxVehiculos)
			maxVehiculos = Obj2[i];
	}


	int iteraciones;
	iteraciones = ITERACIONES;


	for (i = 1; i < iteraciones+1; i++){
		printf("\t\t\t\t\t ITERACION %d\n", i);
		//printf("\t\t\t\t\t PROGRESO: %f\n", (float)i/(float)iteraciones*100);
		genetico(datosGenerales, arregloClientes, i);
	}
	for (i = 0; i < CANTPOBLACION; i++)
	{
		printf("\nIndividuo %d- Distancia: %lf // Vehiculos: %d // Fitness: %.10lf",i+1, Obj1[i], Obj2[i], fitness[i]);
	}
	printf("\n");

	


	Sol prueba;

	prueba = poblacion[0];
	//MOSTRAR SOLUCION GREEDY

	printf("1\n");
	while(prueba!=NULL)
	{
		printf("%d -> ", prueba->num);
		prueba=prueba->sig;
	}
	printf("\n");

		prueba = poblacion[1];
	//MOSTRAR SOLUCION GREEDY

	printf("2\n");
	while(prueba!=NULL)
	{
		printf("%d -> ", prueba->num);
		prueba=prueba->sig;
	}
	printf("\n");

		prueba = poblacion[2];
	//MOSTRAR SOLUCION GREEDY

	printf("3\n");
	while(prueba!=NULL)
	{
		printf("%d -> ", prueba->num);
		prueba=prueba->sig;
	}
	printf("\n");

	printf("Frente de Pareto.\nCantidad de elementos: %d\n", pareto.size());

	for(i=0; i < pareto.size(); i++){
		cout << "Elemento " << i+1 << " objetivo 1: " << pareto[i].obj1 << " obj 2: " << pareto[i].obj2 << endl;
	}
	
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	cout << "Tiempo de ejecucion: " << duration << endl;



	/*
	Sol solucion;

	solucion = greedy(datosGenerales,arregloClientes);

	prueba=solucion;
	//MOSTRAR SOLUCION GREEDY

	printf("MOSTRANDO SOLUCIÓN DEL GREEDY\n");
	while(prueba!=NULL)
	{
		printf("%d -> ", prueba->num);
		prueba=prueba->sig;
	}
	printf("\n");
	prueba=solucion;

	i = restricciones(solucion,arregloClientes,datosGenerales);
	printf("cumple con restricciones: %d\n",i);
	*/
	return 0;
}