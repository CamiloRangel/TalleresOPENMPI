/*
Universidad de Pamplona
Facultad de Ingenierías y Arquitectura
Programa de Ingeniería de Sistemas
Asignatura: Fundamentos de Computación paralela y distribuida
Profesor: Jose Orlando Maldonado Bautista
Autor: Camilo Andrés Camacho Rangel
*/

#include <stdio.h>
#include <mpi.h>


int sumaVector(int v[], int n)
{
   int suma = 0;
   for(int i=0;i<n;i++){
        suma+= v[i];
   }
    	
   return suma;
}

int main(int argc, char *argv[])
{
    int rank, size, *enviado, *recibido, next, from;

    enviado = (int *) malloc (5 * sizeof( int ));
    recibido = (int *) malloc (5 * sizeof( int ));

    MPI_Status estado;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    next = (rank + 1) % size;
    from = (rank + size - 1) % size;
    
    for (int i = 0; i < 5; i++)
    {
        recibido[i] = from;
        enviado[i] = rank;
    }

    if(rank == 0){
        printf("\nEl proceso %d ha recibido los datos de %d y la suma es: %d\n",rank,from,sumaVector(recibido,5));
        MPI_Send(enviado,5,MPI_INT,next,0,MPI_COMM_WORLD);
        MPI_Recv(recibido,5,MPI_INT,from,0,MPI_COMM_WORLD,&estado);
        
    }else{
        
        MPI_Recv(recibido,5,MPI_INT,from,0,MPI_COMM_WORLD,&estado);
        for (int i = 0; i < 5; i++)
        {
        recibido[i] = from;
        }
        printf("\nEl proceso %d ha recibido los datos de %d y la suma es: %d\n",rank,from,sumaVector(recibido,5));
        MPI_Send(recibido,5,MPI_INT,next,0,MPI_COMM_WORLD);
        
        
    }

    MPI_Finalize();
    
    
    return 0;
}