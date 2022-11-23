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



int main(int argc, char *argv[])
{
    
   int rank, size,cant, *enviado, *recibido, max_env, max_rev;
   double resultado_env,resultado_rec;
   cant = 20;

   enviado = (int *)  malloc (cant * sizeof(int));
   recibido = (int *)  malloc (cant * sizeof(int));
   

   MPI_Status estado;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   
   //cant = 5;

   if(rank == 0){

        srand(time(NULL));
        for (int i = 0; i < cant; i++)
        {
            enviado[i] = 1+rand() % 15;
        }

        printf("Vector: \n");
        imprimir(enviado,cant);

        for(int next = 1; next<size;next++){
            MPI_Send(enviado,
                cant,
                MPI_INT,
                next,
                0,
                MPI_COMM_WORLD);
        }

        for (int from = 1; from < size; from++)
        {
            MPI_Recv(&resultado_rec,
                1,
                MPI_DOUBLE,
                from,
                0,
                MPI_COMM_WORLD,
                &estado);
            if(from ==1){
                printf("\nEl proceso %d recibe el promedio del proceso %d: %.2f\n", rank,from,resultado_rec);
            }
            if(from == 2){
                printf("El proceso %d recibe el maximo del proceso %d: %.0f\n", rank, from, resultado_rec);
            }
            if(from == 3){
                 printf("El proceso %d recibe el producto del proceso %d: %.0f\n", rank,from,resultado_rec);
            }
        }
        printf("\n");
        

   }else{
        if(rank == 1){
            MPI_Recv(recibido,
                     cant,
                     MPI_INT,
                     0,
                     0,
                     MPI_COMM_WORLD,
                     &estado);
            
            double sum = 0;
            for(int i = 0; i<cant; i++){
                sum += recibido[i];
            }
            /* printf("Rank = 1\n");
            imprimir(recibido,cant); */
            resultado_env = sum/cant;
            //printf("Promedio proceso 1: %.2f \n", resultado_env);
            MPI_Send(&resultado_env,
                    1,
                    MPI_DOUBLE,
                    0,
                    0,
                    MPI_COMM_WORLD
                    );
        }

        if(rank == 2){
            MPI_Recv(recibido,
                     cant,
                     MPI_INT,
                     0,
                     0,
                     MPI_COMM_WORLD,
                     &estado);
            int max = 0;
            for(int i =0; i<cant; i++){
                if(recibido[i]>max){
                    max = recibido[i];
                }
            }
            //printf("Maximo proceso 2: %d\n", max);
            resultado_env = max;
            /* printf("Rank = 2\n");
            imprimir(recibido,cant);  */
            MPI_Send(&resultado_env,
                    1,
                    MPI_DOUBLE,
                    0,
                    0,
                    MPI_COMM_WORLD
                    );
        }

        if(rank == 3){
            MPI_Recv(recibido,
                     cant,
                     MPI_INT,
                     0,
                     0,
                     MPI_COMM_WORLD,
                     &estado);
            long long int producto = 1;
            for (int i = 0; i < cant; i++)
            {
                producto = producto * recibido[i];
            }
            //printf("Producto proceso 3: %lld\n",producto);
            resultado_env = producto;
            /* printf("Rank = 3\n");
            imprimir(recibido,cant);  */
            MPI_Send(&resultado_env,
                    1,
                    MPI_DOUBLE,
                    0,
                    0,
                    MPI_COMM_WORLD
                    );
        }
   }

 
    

    MPI_Finalize();
    return 0;
}
