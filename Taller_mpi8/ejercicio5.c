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

void llenar(int v[], int n,int max){
    srand(time(NULL));
    int sup = max*6;
    for (int i = 0; i<n; i++){
        v[i] = 1 + rand() % sup;
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
    int rank, size, cant, local;

    MPI_Status estado;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cant = 20;
    local = cant/size;
    int *vector, vectorLocal[local],*recibido;

    vector = (int *)  malloc (cant * sizeof(int));
    recibido = (int *)  malloc (cant * sizeof(int));

    //vector = (int *)  malloc (cant * sizeof(int));
    //vectorLocal = (int *)  malloc (local * sizeof(int));

    if(rank == 0){
        llenar(vectorLocal,local, rank+5);
        //imprimir(vectorLocal,local);
    }else{
        if(rank == 1){
            llenar(vectorLocal,local, rank);
            //imprimir(vectorLocal,local);
            //MPI_Recv(&recibido,1,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
            //imprimir(recibido, cant);
        }

        if(rank == 2){
            llenar(vectorLocal,local, rank);
            //imprimir(vectorLocal,local);
        }

        if(rank == 3){
            llenar(vectorLocal,local, rank);
            //imprimir(vectorLocal,local);
        }
    }

    MPI_Gather(&vectorLocal[0],local,MPI_INT,vector,local,MPI_INT,0,MPI_COMM_WORLD);

    if(rank == 0){
        printf("Vector Final: \n");
        imprimir(vector,cant);
        for (int next = 1; next < size; next++)
        {
            MPI_Send(vector,
            cant,
            MPI_INT,
            next,
            0,
            MPI_COMM_WORLD);
        }

        int max_res;
        long long int producto_res;
        float prom_res;
        MPI_Recv(&prom_res,1,MPI_INT,1,0,MPI_COMM_WORLD,&estado);
        MPI_Recv(&max_res,1,MPI_INT,2,0,MPI_COMM_WORLD,&estado);
        MPI_Recv(&producto_res,1,MPI_LONG_LONG_INT,3,0,MPI_COMM_WORLD,&estado);
        printf("\nEl proceso %d recibe el promedio del proceso %d: %.2f\n", 0,1,prom_res);
        printf("El proceso %d recibe el promedio del proceso %d: %d\n", 0,2,max_res);
        printf("El proceso %d recibe el promedio del proceso %d: %lld\n", 0,3,producto_res);

        
    }else{
        if(rank == 1){
            
            MPI_Recv(recibido,cant,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
            float res = promedio(recibido,cant);
            //printf("Prom: %.2f\n", res);
            MPI_Send(&res,1,MPI_FLOAT,0,0,MPI_COMM_WORLD);
        }
        if(rank == 2){
            
            MPI_Recv(recibido,cant,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
            int maximo = max(recibido,cant);
            //printf("Max: %d\n", maximo);
            MPI_Send(&maximo,1,MPI_INT,0,0,MPI_COMM_WORLD);
        }
        if(rank == 3){
            
            MPI_Recv(recibido,cant,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
            long long int productov = producto(recibido,cant);
            //printf("Producto: %lld\n",productov);
            MPI_Send(&productov,1,MPI_LONG_LONG_INT, 0,0,MPI_COMM_WORLD);
        }
    }
    

    

    MPI_Finalize();
    return 0;
}
