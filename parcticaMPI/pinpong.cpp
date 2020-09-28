#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  const int cant_op = 10;
  MPI_Init(NULL, NULL);
  
  int core_actual;
  MPI_Comm_rank(MPI_COMM_WORLD, &core_actual);
  int cant_cores;
  MPI_Comm_size(MPI_COMM_WORLD, &cant_cores);
  
  cout<<"core "<<core_actual<<" de "<<cant_cores<<endl;

  int contador = 0;
  int core = (core_actual + 1) % 2;
  while (contador < cant_op) {
    if (core_actual == contador % 2) {
      contador++;
    MPI_Send(&contador, 1, MPI_INT, core, 0, MPI_COMM_WORLD);
    cout<<"core "<<core_actual<<" incrementa contador a "<<contador<<" y lo envia al core "<<core<<endl;
    } else {
    MPI_Recv(&contador, 1, MPI_INT, core, 0, MPI_COMM_WORLD,
    MPI_STATUS_IGNORE);
    cout<<"core "<<core_actual<<" recibio contador "<<contador<<" de core "<<core<<endl;
    }
  }
  MPI_Finalize();
}
