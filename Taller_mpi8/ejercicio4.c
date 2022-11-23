/*
Universidad de Pamplona
Facultad de Ingenierías y Arquitectura
Programa de Ingeniería de Sistemas
Asignatura: Fundamentos de Computación paralela y distribuida
Profesor: Jose Orlando Maldonado Bautista
Autor: Camilo Andrés Camacho Rangel
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void imprimir(int v[], int n){
    for (int i = 0; i<n; i++){
        printf("%d\t", v[i]);
    }
    printf("\n");
}

void imprimir_d(float v[], int n){
    for (int i = 0; i<n; i++){
        printf("%.2f\t", v[i]);
    }
    printf("\n");
}

void llenar(int v[], int n){
    srand(time(NULL));
    for (int i = 0; i<n; i++){
        v[i] = 1 + rand() % 15;
    }
}

int max(int v[], int n){
    int max = 0;
    for (int i = 0; i < n; i++)
    {
        if(v[i]>max){
            max = v[i];
        }
    }
    return max;
}

double calpromedio(float v[], int n){
    double  prom,sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += v[i]; 
    }
    prom = sum/n;
    return prom;
}

float promedio(int v[], int n){
    float  prom,sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += v[i]; 
    }
    prom = sum/n;
    return prom;
}

long long int producto(int v[], int n){
    long long int producto = 1;
    for (int i = 0; i < n; i++)
    {
        producto = producto * v[i];
    }
    return producto;
}


int main(int argc, char *argv[])
{

    int rank, size, cant;
    cant = 20;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local = cant/size;
    int Vector[cant], VectorLocal[local];
    float *promedios;
    promedios = (float *)  malloc (size * sizeof(float));
    if(rank == 0){
        llenar(Vector, cant);
        printf("Vector: \n");
        imprimir(Vector, cant);

    }

    MPI_Scatter(&Vector[0],
                    local,
                    MPI_INT,
                    &VectorLocal[0],
                    local,
                    MPI_INT,
                    0,
                    MPI_COMM_WORLD);

    int maximoLocal;
    long long int productoLocal;
    float promedioLocal;

    maximoLocal = max(VectorLocal,local);
    productoLocal = producto(VectorLocal, local);
    promedioLocal = promedio(VectorLocal, local);


    MPI_Gather(&promedioLocal,1,MPI_FLOAT,promedios,1,MPI_FLOAT,0,MPI_COMM_WORLD);

    int maximo;
    long long int producto;
    double promedioMaster;

    MPI_Reduce(&maximoLocal,
               &maximo,
               1,
               MPI_INT,
               MPI_MAX,
               0,
               MPI_COMM_WORLD);
    
    MPI_Reduce(&productoLocal,
               &producto,
               1,
               MPI_LONG_LONG_INT,
               MPI_PROD,
               0,
               MPI_COMM_WORLD);

    
    
    if(rank == 0){
        printf("\nMaximo = %d \n", maximo);
        printf("Producto = %lld \n", producto);


        promedioMaster = calpromedio(promedios, size);
        printf("Promedio: %.2f\n", promedioMaster);
        
    }
    MPI_Finalize();
    
    return 0;
}
