/*
Autor: Camilo Andrés Camacho Rangel
Universidad de Pamplona - Colombia
Fundamentos de computación paralela y distribuida
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void imprimirMatriz(int M[], int f, int c)
{
	for (int i=0;i<f;i++)
	{
		for(int j=0;j<c;j++)
			printf(" %d \t",M[i*c+j]);
		printf("\n");
	}
		
}

void imprimirVector(int V[], int n)
{
	for (int i=0;i<n;i++)
		printf(" %d \t",V[i]);
	printf("\n");
}

void imprimirVectorR(double V[], int n)
{
	for (int i=0;i<n;i++)
		printf(" %.2f \t",V[i]);
	printf("\n");
}

int calMin(int v[], int n){
    int min = 100;
    for (int i = 0; i < n; i++)
    {
        if(v[i]<min){
            min = v[i];
        }
    }
    return min;
}


int calMax(int v[], int n){
    int max = 0;
    for (int i = 0; i < n; i++)
    {
        if(v[i]>max){
            max = v[i];
        }
    }
    return max;
}


double calProm(int v[], int n){
    double sum, prom;
    for (int i = 0; i < n; i++)
    {
        sum += v[i];
    }
    prom = sum / n;

    return prom;
}

void crearMatriz(int M[], int f, int c)
{
	for (int i=0;i<f;i++)
	{
		for(int j=0;j<c;j++)
			M[i*c+j] = 1 * rand() % 20;
	}
		
}


int main(int argc, char *argv[])
{
    int *matriz, NP, N, rank, *vecLocal, *vecMins, *vecMaxs;
    double *vecProm;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("size: %d", NP);
    N = 4;
    matriz = (int *) malloc (NP * N * sizeof(int));
    vecLocal = (int *) malloc (N * sizeof(int));
    vecMins = ( int * ) malloc (NP * sizeof ( int ) );
    vecMaxs = ( int * ) malloc (NP * sizeof ( int ) );
    vecProm = ( double * ) malloc (NP * sizeof ( double ) );

    if (rank == 0)
    {
        
        srand(time(NULL));
        crearMatriz(matriz,NP,N);
        printf("Matriz generado: \n");
        imprimirMatriz(matriz,NP,N); 

    }

    MPI_Scatter(matriz,
                N,
                MPI_INT,
                vecLocal,
                N,
                MPI_INT,
                0,
                MPI_COMM_WORLD);

    /* printf("Rank: %d\n", rank);
    imprimirVector(vecLocal,N); 
    printf("\n"); */
    int minLocal = calMin(vecLocal,N);
    int maxLocal = calMax(vecLocal,N);
    double promLocal = calProm(vecLocal,N);
    
    //printf("Minlocal: %d\n", minLocal);
    /* printf("MaxLocal: %d\n", maxLocal);
    printf("PromLocal: %.2f\n", promLocal); */

    MPI_Gather(&minLocal, 
               1, 
               MPI_INT, 
               vecMins, 
               1, 
               MPI_INT, 
               0, 
               MPI_COMM_WORLD);

    
    MPI_Gather(&maxLocal, 
               1, 
               MPI_INT, 
               vecMaxs, 
               1, 
               MPI_INT, 
               0, 
               MPI_COMM_WORLD);

    
    MPI_Gather(&promLocal, 
               1, 
               MPI_DOUBLE, 
               vecProm, 
               1, 
               MPI_DOUBLE, 
               0, 
               MPI_COMM_WORLD);

    

    if(rank == 0){
        printf("\nVector de minimos: \n");
        imprimirVector(vecMins,NP);

        printf("Vector de maximos: \n");
        imprimirVector(vecMaxs,NP);

        printf("Vector de promedios: \n");
        imprimirVectorR(vecProm,NP);
    }
    
    MPI_Finalize();

    return 0;
}
